#pragma once
#include <unordered_map>
#include "lexer_tokens.h"
typedef struct
{
    token_class type;
    bool known=false;// is the value known in compilation time. (used for compile time evaluation)
    union
    {
        char byte_value;
        long long int int_value;
        double real_value;
    };
} symbol_entry;
typedef struct
{
    int scope = 0;                                                                       // the scope we are in, goes up
    int label = 1;                                                                       // the label id to be generated (avoid repeated labels)
    std::unordered_map<int, std::unordered_map<std::string, symbol_entry>> symbol_table; // int key is scope, string key is name.
} semantic_struct;
typedef bool (*semantic_rule)(semantic_struct &, const token &input, const token &stack_top);
bool set_type(semantic_struct &state, const token &input, const token &stack_top);