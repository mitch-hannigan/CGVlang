#include "error_helper.h"
std::string tokenClassToString(token_class tclass)
{
    if (tclass & token_non_terminal)
        return "non terminal";
    switch (tclass)
    {
    case token_eof:
        return "eof";
    case token_unknown:
        return "unknown";
    case token_logic_operator:
        return "logic_operator";
    case token_arithmetic_operator:
        return "arithmetic_operator";
    case token_comparison_operator:
        return "comparison_operator";
    case token_marker:
        return "marker";
    case token_id:
        return "id";
    case token_keyword:
        return "keyword";
    case token_type:
        return "type";
    case token_val:
        return "val";
    case token_left_paren:
        return "left_paren";
    case token_right_paren:
        return "right_paren";
    case token_left_brace:
        return "left_brace";
    case token_right_brace:
        return "right_brace";
    case token_comma:
        return "comma";
    case token_plus:
        return "plus";
    case token_minus:
        return "minus";
    case token_divide:
        return "divide";
    case token_multiply:
        return "multiply";
    case token_exponent:
        return "exponent";
    case token_assign:
        return "assign";
    case token_and:
        return "and";
    case token_or:
        return "or";
    case token_not:
        return "not";
    case token_equal:
        return "equal";
    case token_less_than:
        return "less_than";
    case token_less_than_equal:
        return "less_than_equal";
    case token_greater_than_equal:
        return "greater_than_equal";
    case token_greater_than:
        return "greater_than";
    case token_get:
        return "get";
    case token_put:
        return "put";
    case token_doif:
        return "doif";
    case token_maybe_not:
        return "maybe_not";
    case token_do_it_again:
        return "do_it_again";
    case token_do_it_again_until:
        return "do_it_again_until";
    case token_type_byte:
        return "type_byte";
    case token_type_int:
        return "type_int";
    case token_type_real:
        return "type_real";
    case token_val_byte:
        return "val_byte";
    case token_val_char:
        return "val_char";
    case token_val_int:
        return "val_int";
    case token_val_real:
        return "val_real";
    case token_error:
        return "error";
    default:
        return "unknown";
    }
}
std::string get_expected_tokens(const std::unordered_map<token_class, std::pair<std::list<token_class>, semantic_rule>> &line)
{
    std::stringstream final;
    for (const auto [key, value] : line)
    {
        if (value.first.size())
            final << tokenClassToString(key) << ", ";
    }
    return final.str();
}
token_class t_class_basic(token_class tok)
{
    if (tok & token_val)
        return token_val;
    if (tok & token_type)
        return token_type;
    return tok;
}
std::string get_tac_type(token_class tok)
{
    std::string final;
    if((tok & token_type_byte) == token_type_byte) final=" u8 ";
    else if((tok & token_type_int) == token_type_int) final=" i64 ";
    else if((tok & token_type_real) == token_type_real) final=" f64 ";
    return final;
}