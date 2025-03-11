#include "semantic.h"
#include "error_helper.h"
void write_future_code(semantic_struct &state)
{
    if (state.future_code.size()) // we can write code, not having code to write is an error, but a sintatic one, so we don't care.
    {
        state.code += state.future_code.top();
        state.future_code.pop();
    }
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
                state.code += (type & token_type_int) == token_type_int ? "read_int()" : (type & token_type_byte) == token_type_byte ? "read_string(1u32)"
                                                                                                                                     : "// not implemented read_f64";
                state.code += " ";
            }
        }
    }
    return true;
}