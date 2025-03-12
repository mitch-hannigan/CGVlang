#include <iostream>
#include "semantic.h"
#include "error_helper.h"
#include "tac_helper.h"
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
void expr_solve_boolean(semantic_struct &state, const std::string &op_text)
{
    expr_solver &solver = state.sub_expr.top();
    std::string temp_name = get_temporary_name(state);
    std::string temp_name2 = get_temporary_name(state);
    symbol_entry entry;
    entry.val = temp_name2;
    if (solver.op.tclass == token_not) // not is a special case.
    {
        state.code += temp_name + std::string(": bool = ") + op_text + std::string("(bool) ") + get_tac_text_form(solver.first);
        entry.type = solver.first.tclass;
        state.code += generate_declaration(entry) + std::string("= ") + generate_tac_cast_code(solver.first.tclass) + temp_name + " ";
    }
    else if (solver.op.tclass & token_logic_operator) // logic ones, we have to cast.
    {
        state.code += temp_name + std::string(": bool = ");
        state.code += "(bool ) ";
        state.code += get_tac_text_form(solver.first);
        state.code += op_text;
        state.code += "(bool ) ";
        state.code += get_tac_text_form(solver.second);
        entry.type = get_highest_precision(solver.first.tclass, solver.second.tclass);
        state.code += generate_declaration(entry) + std::string("= ") + generate_tac_cast_code(get_highest_precision(solver.first.tclass, solver.second.tclass)) + temp_name + " ";
    }
    else // all remaining ones
    {
        state.code += temp_name + std::string(": bool = ");
        if (get_highest_precision(solver.first.tclass, solver.second.tclass) != solver.first.tclass) // cast necessário aqui
            state.code += generate_tac_cast_code(solver.second.tclass);
        state.code += get_tac_text_form(solver.first);
        state.code += op_text;
        if (get_highest_precision(solver.first.tclass, solver.second.tclass) != solver.second.tclass) // cast necessário aqui
            state.code += generate_tac_cast_code(solver.first.tclass);
        state.code += get_tac_text_form(solver.second);
        entry.type = get_highest_precision(solver.first.tclass, solver.second.tclass);
        state.code += generate_declaration(entry) + std::string("= ") + generate_tac_cast_code(get_highest_precision(solver.first.tclass, solver.second.tclass)) + temp_name + " ";
    }
    solver.first.text = temp_name2;
    solver.first.tclass = entry.type;
    solver.op.tclass = token_unknown;
    solver.second.tclass = token_unknown;
}
void expr_solve_arithmetic(semantic_struct &state, const std::string &op_text)
{
    expr_solver &solver = state.sub_expr.top();
    symbol_entry entry;
    if (solver.op.tclass == token_assign) // atribution is a special case.
    {
        entry.val = solver.first.text;
        entry.type = get_pure_type(solver.first.tclass);
        state.code += entry.val + " ";
        state.code += op_text;
        if (entry.type != get_pure_type(solver.second.tclass)) // cast necessário aqui
        {
            state.code += generate_tac_cast_code(solver.first.tclass);
            if (get_highest_precision(solver.first.tclass, solver.second.tclass) != solver.first.tclass) // narrower mode warning
                std::cout << "warning: you are assigning a variable to a narrower one, this may cause problems. line " << solver.op.line << " col " << solver.op.col << "\n";
        }
        state.code += get_tac_text_form(solver.second);
    }
    else // all remaining ones
    {
        entry.val = get_temporary_name(state);
        entry.type = get_pure_type(get_highest_precision(solver.first.tclass, solver.second.tclass));
        state.code += generate_declaration(entry) + std::string("= ");
        if (entry.type != get_pure_type(solver.first.tclass)) // cast necessário aqui
            state.code += generate_tac_cast_code(solver.second.tclass);
        state.code += get_tac_text_form(solver.first);
        state.code += op_text;
        if (entry.type != get_pure_type(solver.second.tclass)) // cast necessário aqui
            state.code += generate_tac_cast_code(solver.first.tclass);
        state.code += get_tac_text_form(solver.second);
    }
    solver.first.text = entry.val;
    solver.first.tclass = entry.type;
    solver.op.tclass = token_unknown;
    solver.second.tclass = token_unknown;
}
void expr_solve(semantic_struct &state)
{
    expr_solver &solver = state.sub_expr.top();
    if ((solver.op.tclass & token_comparison_operator) || (solver.op.tclass & token_logic_operator)) // is boolean
    {
        if (solver.first.tclass != token_unknown && (solver.second.tclass != token_unknown || solver.op.tclass == token_not))
            expr_solve_boolean(state, get_tac_operator(solver.op.tclass));
    }
    else if ((solver.op.tclass & token_arithmetic_operator) == token_arithmetic_operator) // is arithmetic
    {
        if (solver.first.tclass != token_unknown && solver.second.tclass != token_unknown)
            expr_solve_arithmetic(state, get_tac_operator(solver.op.tclass));
    }
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
    state.current_entry.val = tokens[token_index].text;
    state.symbol_table[state.scope][tokens[token_index].text] = state.current_entry;
    auto tok = make_token_from_entry(state.current_entry);
    tok.line = tokens[token_index].line;
    tok.col = tokens[token_index].col;
    state.future.push(future_action{{tok}, declare_tac_variable});
    return true;
}
bool expr_set_operand_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top)
{
    token tok = tokens[token_index];
    auto operand = make_entry_from_token(state, tok);
    if (operand.type == token_unknown) // error, will happen if symbol is not found in the table.
        return false;
    tok.tclass = operand.type;
    state.sub_expr.top().first = tok; // this is a leaf, allways left operand.
    return true;
}
bool expr_set_operator_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top)
{
    if (state.sub_expr.top().second.tclass != token_unknown) // this node has more than two children, pre resolving it to get back to 1.
        expr_solve(state);                                   // try to solve
    if (state.sub_expr.top().op.tclass != token_unknown)     // error: ~id=expr
    {
        state.error = "operator is set to: " + state.sub_expr.top().op.text;
        return false;
    }
    state.sub_expr.top().op = tokens[token_index];
    return true;
}
bool expr_move_down_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top)
{
    state.sub_expr.push(expr_solver()); // by default the solver is started with unknown, that's what we want, we are going down in the tree, so empty nodes for now, will propagate up.
    return true;
}
bool expr_move_up_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top)
{
    expr_solve(state);                              // try to solve
    expr_solver last_solver = state.sub_expr.top(); // this is the current node, let's propagate it.
    state.sub_expr.pop();
    if (state.sub_expr.size()) // let's propagate
    {
        expr_solver &solver = state.sub_expr.top();
        if (solver.first.tclass == token_unknown) // we are in a sub_expr path, will merge eventually, or just resolve to val/id.
            solver.first = last_solver.first;
        else // merged
            solver.second = last_solver.first;
        // we don't propagate operator up.
    }
    else // we will propagate the final result. (the expr is resolved, this will save it for code generation, but only till next expr is evaluated)
    {
        state.last_solved_expr = symbol_entry{last_solver.first.tclass, last_solver.first.text}; // we do it like this because we are not allowed to search in the table for the symbol. (if it's an id we already checked for it, and for temporary ones, no need to check, the compiler will alocate them)
        state.temporary = 1;                                                                     // temps will be regenerated for next expr.
        return activate_action(state, tokens, token_index, stack_top, expr_evaluated_trigger);
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
    if (activate_action(state, tokens, token_index, stack_top, end_of_scope_trigger)) // action run ok.
    {
        state.symbol_table.erase(state.scope);
        state.scope--;
        return true;
    }
    return false;
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
                return false;
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
token_class get_pure_type(token_class tclass)
{
    return token_class(tclass & token_type_any);
}
bool activate_action(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top, future_action_trigers trigger)
{
    state.trigger = trigger;
    if (state.future.size()) // we have actions to run
    {
        future_action action = state.future.top();
        state.future.pop();
        if (!action.action) // code writer
            for (auto &tok : action.data)
                state.code += tok.text + " ";
        else // action is a function
        {
            return action.action(state, tokens, token_index, stack_top, action.data);
        }
    }
    return true;
}
bool demand_action_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top)
{
    return activate_action(state, tokens, token_index, stack_top, on_demand_trigger);
}
token make_token_from_entry(const symbol_entry &entry)
{
    token final;
    final.text = entry.val;
    final.tclass = entry.type;
    return final;
}
bool declare_tac_variable(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top, std::vector<token> &data)
{
    auto entry = make_entry_from_token(state, data[0]); // this should work fine, and it'll never faill, in theory.
    state.code += generate_declaration(entry) + "= ";
    if (state.trigger == expr_evaluated_trigger)
    {
        if (get_pure_type(entry.type) != get_pure_type(state.last_solved_expr.type)) // cast needed
            state.code += generate_tac_cast_code(entry.type);
        state.code += get_tac_text_form(token{state.last_solved_expr.type, state.last_solved_expr.val, 0, 0});
        if (get_highest_precision(get_pure_type(entry.type), get_pure_type(state.last_solved_expr.type)) != get_pure_type(entry.type)) // warning
            std::cout << "warning: Storing value in a narrower variable, this may cause problems. line " << data[0].line << " col " << data[0].col << std::endl;
    }
    else
    {
        state.code += "0";
        if ((entry.type & token_type_real) == token_type_real)
            state.code += ".0";
        state.code += get_tac_type(entry.type);
    }
    return true;
}
bool put_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top)
{
    future_action action;
    action.action = generat_tac_put;
    state.future.push(action);
    return true;
}
bool generat_tac_put(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top, std::vector<token> &data)
{
    state.code += "call write_" +
                  get_tac_type(state.last_solved_expr.type) + std::string("(") + get_tac_text_form(make_token_from_entry(state.last_solved_expr)) + std::string(") ");
    return true;
}