#include "error_helper.h"
#include "lr_parser.h"
#include <stack>
#include <iostream>
void lr_parse(const std::vector<token> &tokens, bool &result)
{
    result = true;
    std::stack<token_class> lr_stack;
    lr_stack.push(token_eof);
    lr_stack.push(token_non_s);
    int token_index = 0;
    while (token_index < tokens.size())
    {
        if (lr_stack.top() & token_non_terminal)
        {
            std::unordered_map<token_class, std::list<token_class>> &line = lr_table[lr_stack.top()];
            if (line.find(tokens[token_index].tclass) != line.end())
            {
                auto temp = line[tokens[token_index].tclass];
                if (temp.size()) // no error
                {
                    lr_stack.pop();
                    for (auto it = temp.rbegin(); it != temp.rend(); it++)
                    {
                        lr_stack.push(*it);
                    }
                }
                else // sinc found
                {
                    std::cout << "error: found " << tokenClassToString(tokens[token_index].tclass) << "expected: " << get_expected_tokens(line) << ". on line " << tokens[token_index].line << " on col " << tokens[token_index].col << std::endl;
                    result = false;
                }
            }
            else // error, empty table for this terminal
            {
                std::cout << "error: found " << tokenClassToString(tokens[token_index].tclass) << "expected: " << get_expected_tokens(line) << ". on line " << tokens[token_index].line << " on col " << tokens[token_index].col << std::endl;
                result = false;
            }
        }
        else // top is a terminal too
        {
            if (lr_stack.top() == tokens[token_index].tclass) // terminal match
            {
                lr_stack.pop();
                token_index++;
            }
            else // error terminal miss match
            {
                std::cout << "error: expected " << tokenClassToString(lr_stack.top()) << " found " << tokenClassToString(tokens[token_index].tclass) << " on line " << tokens[token_index].line << " col " << tokens[token_index].col << std::endl;
                result = false;
                lr_stack.pop();
            }
        }
    }
}