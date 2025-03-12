#pragma once
#include <unordered_map>
#include <string>
#include <stack>
#include <vector>
#include "lexer_tokens.h"
struct semantic_struct;
typedef bool (*semantic_rule)(semantic_struct &, const std::vector<token> &tokens, int token_index, const token_class &stack_top);
struct future_action
{
    std::string code; // the code to be added to the tac code
};
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
    std::stack<std::string> future_code;                                                 // code that should be added latter (loops and ifs)
    std::stack<expr_solver> sub_expr;                                                  // used for recursive expr resolution. (aka bottom up nodes propagation)
};
symbol_entry make_entry_from_token(semantic_struct &state, const token &tok);
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