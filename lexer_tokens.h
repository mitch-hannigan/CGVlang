#pragma once
enum{
    // simple token flags
    token_unknown=0,
    token_logic_operator = 0x1,
    token_arithmetic_operator = 0x2,
    token_comparison_operator = 0x4,
    token_marker=0x8,
    token_id = 0x10,
    token_key_word = 0x20,
    //composed tokens
    token_left_paren = (token_arithmetic_operator | token_comparison_operator | token_logic_operator) | 0x80000000,
    token_right_paren= (token_arithmetic_operator | token_comparison_operator | token_logic_operator) | 0x40000000,
    token_left_brace = token_marker | 0x80000000,
    token_right_brace = token_marker | 0x40000000,
    token_plus = token_arithmetic_operator | 0x80000000,
    token_minus = token_arithmetic_operator | 0x40000000,
    token_divide = token_arithmetic_operator | 0x20000000,
    token_multiply = token_arithmetic_operator | 0x10000000,
    token_exponent = token_arithmetic_operator | 0x08000000,
    token_assign = token_arithmetic_operator | 0x04000000,
}token_class;