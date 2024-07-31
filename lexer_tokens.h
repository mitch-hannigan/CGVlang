#pragma once
#include <string>
enum token_class
{
    // simple token flags
    token_unknown = 0,
    token_logic_operator = 0x1,
    token_arithmetic_operator = 0x2,
    token_comparison_operator = 0x4,
    token_marker = 0x8,
    token_id = 0x10,
    token_keyword = 0x20,
    token_type = 0x40,
    token_val = 0x80,
    // composed tokens
    token_left_paren = token_marker | 0x80000000,
    token_right_paren = token_marker | 0x40000000,
    token_left_brace = token_marker | 0x20000000,
    token_right_brace = token_marker | 0x10000000,
    token_comma = token_marker | 0x08000000,
    token_plus = token_arithmetic_operator | 0x80000000,
    token_minus = token_arithmetic_operator | 0x40000000,
    token_divide = token_arithmetic_operator | 0x20000000,
    token_multiply = token_arithmetic_operator | 0x10000000,
    token_exponent = token_arithmetic_operator | 0x08000000,
    token_assign = token_arithmetic_operator | 0x04000000,
    token_and = token_logic_operator | 0x80000000,
    token_or = token_logic_operator | 0x40000000,
    token_not = token_logic_operator | 0x20000000,
    token_equal = token_comparison_operator | 0x80000000,
    token_less_than = token_comparison_operator | 0x40000000,
    token_less_than_equal = token_comparison_operator | 0x20000000,
    token_greater_than_equal = token_comparison_operator | 0x10000000,
    token_greater_than = token_comparison_operator | 0x08000000,
    token_get = token_keyword | 0x80000000,
    token_put = token_keyword | 0x40000000,
    token_doif = token_keyword | 0x20000000,
    token_maybe_not = token_keyword | 0x10000000,
    token_do_it_again = token_keyword | 0x08000000,
    token_do_it_again_until = token_keyword | 0x04000000,
    token_type_byte = token_type | 0x80000000,
    token_type_int = token_type | 0x40000000,
    token_type_real = token_type | 0x20000000,
    token_val_byte = token_type_byte | token_val,
    token_val_char = token_val_byte | 0x10000000,// 0x80000000to0x20000000 is already in use in this case.
    token_val_int = token_type_int | token_val,
    token_val_real = token_type_real | token_val,
    token_error = 0xffffffff
};
typedef struct
{
    token_class tclass = token_unknown;
    std::string text;
    unsigned long long line, col;
} token;