#pragma once
#include <unordered_map>
#include <string>
#include <stack>
#include <vector>
#include "lexer_tokens.h"
enum future_action_trigers
{
    end_of_scope_trigger,
    expr_evaluated_trigger,
    on_demand_trigger
};
struct semantic_struct;
typedef bool (*semantic_rule)(semantic_struct &, const std::vector<token> &tokens, int token_index, const token_class &stack_top);
typedef bool (*action_function)(semantic_struct &, const std::vector<token> &tokens, int token_index, const token_class &stack_top, std::vector<token> &data);
struct symbol_entry
{
    token_class type = token_unknown;
    std::string val; // the id or value. (will allways be variable name for symbols in the table.)
};
struct expr_solver
{
    token first;  // operand 1
    token op;     // operator
    token second; // operand 2
};
struct future_action
{
    std::vector<token> data;          // can be used to store information for the future function. (if function is null the code will write what's present on this vector, the text of the tokens, from first to last.)
    action_function action = nullptr; // by default this is a future code writer
};
struct semantic_struct
{
    int scope = 0;                                                                       // the scope we are in, goes up or down, (can't go below 0)
    int label = 1;                                                                       // the label id to be generated (avoid repeated labels)
    int temporary = 1;                                                                   // what temporary variable to use.
    std::unordered_map<int, std::unordered_map<std::string, symbol_entry>> symbol_table; // int key is scope, string key is name.
    symbol_entry current_entry;                                                          // symbol to be added to the table.
    symbol_entry last_solved_expr;                                                       // the last solved expr, needed because we move to parent nodes when finished expression processing.
    std::string error;                                                                   // the error to be printed.
    std::string code;                                                                    // the etac code being generated.
    std::stack<future_action> future;                                                    // can write pre generated code or call semantic functions (activates every time a scope ends or an expr finishes it's evaluation.)
    future_action_trigers trigger;                                                       // the reason the action has been activated.
    std::stack<expr_solver> sub_expr;                                                    // used for recursive expr resolution. (aka bottom up nodes propagation)
};
symbol_entry make_entry_from_token(semantic_struct &state, const token &tok);
token make_token_from_entry(const symbol_entry&entry);
int get_symbol_scope(semantic_struct &state, const token &tok);
void expr_solve_boolean(semantic_struct &state, const std::string &op_text);
void expr_solve_arithmetic(semantic_struct &state, const std::string &op_text);
void expr_solve(semantic_struct &state);
bool set_type(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top);
bool add_symbol(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top);
bool expr_set_operand_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top);
bool expr_set_operator_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top);
bool expr_move_down_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top);
bool expr_move_up_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top);
bool begin_scope(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top);
bool end_scope(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top);
bool get_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top);
token_class get_pure_type(token_class tclass);
bool activate_action(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top, future_action_trigers trigger);
bool demand_action_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top);
bool declare_tac_variable(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top, std::vector<token> &data);
bool generat_tac_put(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top, std::vector<token> &data);
bool put_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top);