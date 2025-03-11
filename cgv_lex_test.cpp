#include "error_helper.h"
#include "lexer.h"
#include "file_reader.h"
#include <iostream>
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
void print_tokens(const std::vector<token> &tokens)
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