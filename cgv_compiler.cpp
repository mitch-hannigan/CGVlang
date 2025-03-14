#include "ll_parser.h"
#include "error_helper.h"
#include "lexer.h"
#include "file_reader.h"
#include <iostream>
void compile(std::string &source)
{
    bool continue_process;
    auto tokens = lex_analyze(source, continue_process);
    if (!continue_process)
    {
        std::cout << "lexical analysis failed, stoping";
        return;
    }
    auto str = ll_parse(tokens, continue_process);
    std::cout << str << '\n';
    if (!continue_process)
    {
        std::cout << "you have a sintatic or semantic error, bad times.";
        return;
    }
    std::cout << "good times, ll parsing successfull";
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