#include <iostream>
#include "semantic.h"
#include "error_helper.h"
#include "tac_helper.h"
void write_future_code(semantic_struct &state)
{
    if (state.future_code.size()) // we can write code, not having code to write is an error, but a sintatic one, so we don't care.
    {
        state.code += state.future_code.top();
        state.future_code.pop();
    }
}
std::string get_temporary_name(semantic_struct &state)
{
    return std::string("t") + std::to_string(state.temporary++);
}
std::string get_label_name_prefix(semantic_struct &state)
{
    return std::string("l") + std::to_string(state.label++);
}
int get_symbol_scope(semantic_struct &state, const token &tok)
{
    if ((tok.tclass & token_id) == token_id)                     // true if the token is an id.
        for (int i = state.symbol_table.size() - 1; i >= 0; i--) // from top scope to first scope.
        {
            if (state.symbol_table[i].find(tok.text) != state.symbol_table[i].end()) // symbol found on current scope
                return i;
        }
    state.error = "No symbol named " + tok.text;
    return -1; // not found
}
symbol_entry make_entry_from_token(semantic_struct &state, const token &tok)
{
    symbol_entry final;
    if (tok.tclass == token_id)
    {
        int scope = get_symbol_scope(state, tok);
        if (scope != -1) // found symbol
        {
            final = state.symbol_table[scope][tok.text];
        }
    }
    else
    {
        final.type = tok.tclass;
        final.val = tok.text;
    }
    return final;
}
bool set_type(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top)
{
    state.current_entry.type = tokens[token_index].tclass;
    return true;
}
bool add_symbol(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top)
{
    if (get_symbol_scope(state, tokens[token_index]) == state.scope) // error
    {
        state.error = std::string("symbol ") + tokens[token_index].text + " already exists in the current scope.";
        return false;
    }
    state.symbol_table[state.scope][tokens[token_index].text] = state.current_entry;
    state.code += tokens[token_index].text + std::string(":") + get_tac_type(state.current_entry.type);
    return true;
}
void expr_add_operand(semantic_struct &state, const token &operand)
{
    if (state.solver.first.tclass == token_unknown)
    {
        state.solver.first = operand;
    }
    else if (state.solver.second.tclass == token_unknown)
    {
        state.solver.second = operand;
    }
}
bool expr_set_operand_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top)
{
    token tok = tokens[token_index];
    auto operand = make_entry_from_token(state, tok);
    if (operand.type == token_unknown) // error
        return false;
    tok.tclass = operand.type;
    expr_add_operand(state, tok);
    return true;
}
bool expr_set_operator_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top)
{
    if (state.solver.op.tclass == token_unknown) // we don't know the operator yet
        state.solver.op = tokens[token_index];
    else // we already have an operator, going down in the stack.
    {
        state.sub_expr.push(state.solver);
        // reset the expr solver.
        state.solver.first.tclass = token_unknown;
        state.solver.second.tclass = token_unknown;
        state.solver.op.tclass = token_unknown;
    }
    return true;
}
void expr_solve_logic(semantic_struct &state, const std::string &op_text)
{
    expr_solver &solver = state.solver;
    std::string temp_name = get_temporary_name(state);
    std::string temp_name2 = get_temporary_name(state);
    if (solver.op.tclass == token_not) // not is a special case.
    {
        state.code += temp_name + std::string(": bool = (bool) ") + get_tac_text_form(solver.first) + op_text;
        state.code += temp_name2 + std::string(": ") + get_tac_type(solver.first.tclass) + std::string("= ") + generate_tac_cast_code(solver.first.tclass) + temp_name + " ";
    }
    else // && or ||
    {
    }
    solver.first.text = temp_name2;
}
bool expr_done_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top)
{
    symbol_entry &last_expr = state.last_solved_expr; // where's the last solved expr?
    expr_solver *solver_ptr = &(state.solver);
    while (solver_ptr->first.tclass != token_unknown) // will be false when fully reset
    {
        expr_solver &solver = *solver_ptr;
        std::string op_text = get_tac_operator(solver.op.tclass); // the operator instruction in tac form
        if (solver.op.tclass == token_unknown)                    // solve to value or id (no op)
        {
        }
        else if (solver.op.tclass == token_assign) // solve the = operator
        {
            state.code += solver.first.text + std::string(" ") + op_text + std::string(" ");
            if (solver.first.tclass != solver.second.tclass) // cast needed
            {
                if (get_highest_precision(solver.first.tclass, solver.second.tclass) != solver.first.tclass) // warning
                    std::cout << "warning: trying to assign to a narrower variable (expect loss of precision), on line " << solver.op.line << " col " << solver.op.col << std::endl;
                state.code += generate_tac_cast_code(solver.first.tclass);
            }
            state.code += solver.second.text + std::string(" ");
        }
        else if (solver.op.tclass == token_not || solver.op.tclass == token_or || solver.op.tclass == token_and) // solve logic operators
            expr_solve_logic(state, op_text);
        last_expr = make_entry_from_token(state, solver.first);
        if (state.sub_expr.size()) // we have stuff to process
        {
            *solver_ptr = state.sub_expr.top();
            state.sub_expr.pop();
        }
        else
        {
            // reset the expr solver.
            state.solver.first.tclass = token_unknown;
            state.solver.second.tclass = token_unknown;
            state.solver.op.tclass = token_unknown;
            state.temporary = 1;
        }
    }
    return true;
}
bool begin_scope(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top)
{
    state.scope++;
    return true;
}
bool end_scope(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top)
{
    state.symbol_table.erase(state.scope);
    state.scope--;
    write_future_code(state);
    return true;
}
bool get_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top)
{
    int temp_index = token_index + 1;
    if (temp_index < tokens.size()) // true if we have more tokens after the (
    {
        if ((tokens[temp_index].tclass & token_id) == token_id) // we have an id, if not it's an error, but a sintatic one.
        {
            int scope = get_symbol_scope(state, tokens[temp_index]);
            if (scope == -1) // not found
            {
                state.error = tokens[temp_index].text + " is not available in the current scope.";
                return false;
            }
            else // generating code
            {
                token_class type = state.symbol_table[scope][tokens[temp_index].text].type;
                state.code += tokens[temp_index].text + std::string(" = call ");
                state.code += (type & token_type_int) == token_type_int ? "read_int64()" : (type & token_type_byte) == token_type_byte ? "read_string(1u32)"
                                                                                                                                       : "read_f64";
                state.code += " ";
            }
        }
    }
    return true;
}