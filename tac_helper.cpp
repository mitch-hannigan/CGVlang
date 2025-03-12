#include "tac_helper.h"
std::string get_tac_text_form(const token &tok)
{
    if (tok.tclass == token_id)
        return tok.text + " ";
    return tok.text + get_tac_type(tok.tclass);
}
std::string get_tac_type(token_class tok)
{
    if ((tok & token_type_byte) == token_type_byte)
        return "u8 ";
    else if ((tok & token_type_int) == token_type_int)
        return "i64 ";
    else if ((tok & token_type_real) == token_type_real)
        return "f64 ";
    return "error ";
}
std::string get_tac_operator(token_class tclass)
{
    switch (tclass)
    {
    case token_plus:
        return "+ ";
    case token_minus:
        return "- ";
    case token_divide:
        return "/ ";
    case token_multiply:
        return "* ";
    case token_exponent:
        return "** ";
    case token_assign:
        return "= ";
    case token_and:
        return "and ";
    case token_or:
        return "or ";
    case token_not:
        return "not ";
    case token_equal:
        return "== ";
    case token_less_than:
        return "< ";
    case token_less_than_equal:
        return "<= ";
    case token_greater_than_equal:
        return ">= ";
    case token_greater_than:
        return "> ";
    }
    return "error ";
}
std::string generate_tac_cast_code(token_class tclass)
{
    return std::string("(") + get_tac_type(tclass) + std::string(") ");
}
std::string generate_declaration(symbol_entry entry)
{
    return entry.val + std::string(": ") + get_tac_type(entry.type);
}