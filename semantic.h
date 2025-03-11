#pragma once
#include <unordered_map>
#include <string>
#include <stack>
#include <vector>
#include "lexer_tokens.h"
typedef struct
{
    token_class type;
    std::string val; // the id or value. (will allways be variable name for symbols in the table.)
} symbol_entry;
typedef struct s_expr_solver
{
    symbol_entry first;// operand 1
    token_class op;// operator
    symbol_entry second;// operand 2
    std::stack<s_expr_solver> sub_expr;// used for recursive expr resolution.
} expr_solver;
typedef struct
{
    int scope = 0;                                                                       // the scope we are in, goes up or down, (can't go below 0)
    int label = 1;                                                                       // the label id to be generated (avoid repeated labels)
    int temporary=1; // what temporary variable to use.
    std::unordered_map<int, std::unordered_map<std::string, symbol_entry>> symbol_table; // int key is scope, string key is name.
    symbol_entry current_entry;                                                          // symbol to be added to the table.
    symbol_entry last_solved_expr;                                                       // the last solved expr, needed because we move to parent nodes when finished expression processing.
    std::string error;                                                                   // the error to be printed.
    std::string code;                                                                    // the etac code being generated.
    std::stack<std::string> future_code;                                                 // code that should be added latter (loops and ifs)
    expr_solver recursive_solver;// recursive expr solver. (that's how we get values from children nodes)
} semantic_struct;
typedef bool (*semantic_rule)(semantic_struct &, const std::vector<token> &tokens, int token_index, const token_class &stack_top);
int get_symbol_scope(semantic_struct &state, const token &tok);
bool set_type(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top);
bool add_symbol(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top);
bool begin_scope(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top);
bool end_scope(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top);
bool get_rule(semantic_struct &state, const std::vector<token> &tokens, int token_index, const token_class &stack_top);