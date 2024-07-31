#include "lexer.h"
#include "file_reader.h"
#include <iostream>
std::string tokenClassToString(token_class tclass)
{
    switch (tclass)
    {
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
std::string generic_token_classes_to_string(token_class tclass)
{
    std::string result;
    if (tclass & token_logic_operator)
        result += "logic_operator ";
    if (tclass & token_arithmetic_operator)
        result += "arithmetic_operator ";
    if (tclass & token_comparison_operator)
        result += "comparison_operator ";
    if (tclass & token_marker)
        result += "marker ";
    if (tclass & token_keyword)
        result += "keyword ";
    if (tclass & token_type)
        result += "type ";
    if (tclass & token_val)
        result += "val ";
    if (tclass & token_id)
        result += "id";
    return result.empty() ? "none" : result;
}
void print_tokens(const std::list<token> &tokens)
{
    std::cout << "line" << '\t' << "col" << '\t' << "origin text" << '\t' << "token class flag" << '\t' << "generic token classes" << '\t' << "token class" << std::endl;
    for (auto t : tokens)
    {
        std::cout << t.line << '\t' << t.col << '\t' << t.text << '\t' << std::hex << t.tclass << std::dec << '\t' << generic_token_classes_to_string(t.tclass) << '\t' << tokenClassToString(t.tclass) << std::endl;
    }
}
void compile(std::string &source)
{
    bool continue_process;
    auto tokens = lex_analyze(source, continue_process);
    print_tokens(tokens);
}
int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        std::cerr << "Please pass source file name" << std::endl;
        return -1;
    }
    try
    {
        auto source = readFile(argv[1]);
        compile(source);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}