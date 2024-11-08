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
    while (token_index < tokens.size() && lr_stack.size())
    {
        if (lr_stack.top() & token_non_terminal)
        {
            std::unordered_map<token_class, std::list<token_class>> &line = lr_table[lr_stack.top()];
            if (line.find(t_class_basic(tokens[token_index].tclass)) != line.end())
            {
                auto temp = line[t_class_basic(tokens[token_index].tclass)];
                if (temp.size()) // no error
                {
                    lr_stack.pop();
                    for (auto it = temp.rbegin(); it != temp.rend(); it++)
                    {
                        if (*it != token_eof)// se for eof não produz nada.
                            lr_stack.push(*it);
                    }
                }
                else // sinc found
                {
                    std::cout << "error: found " << tokenClassToString(tokens[token_index].tclass) << " expected: " << get_expected_tokens(line) << ". on line " << tokens[token_index].line << " on col " << tokens[token_index].col << " trying sinc."<< std::endl;
                    lr_stack.pop();
                    result = false;
                }
            }
            else // error, empty table for this terminal
            {
                std::cout << "error: found " << tokenClassToString(tokens[token_index].tclass) << " expected: " << get_expected_tokens(line) << ". on line " << tokens[token_index].line << " on col " << tokens[token_index].col << " empty table" << std::endl;
                token_index++;
                result = false;
            }
        }
        else // top is a terminal too
        {
            if (lr_stack.top() == t_class_basic(tokens[token_index].tclass)) // terminal match
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


std::unordered_map<token_class, std::unordered_map<token_class, std::list<token_class>>> lr_table =
    {
        // main hash_map
        {
            // first line
            token_non_s,
            {// begin line content
             {token_type, {token_type, token_non_typedecl, token_non_s}},
             {token_get, {token_get, token_non_getfunc, token_non_s}},
             {token_put, {token_put, token_non_putfunc, token_non_s}},
             {token_doif, {token_doif, token_non_doif, token_non_s}},
             {token_do_it_again, {token_do_it_again, token_non_do_it_again, token_non_s}},
             {token_do_it_again_until, {token_do_it_again_until, token_non_do_it_again_until, token_non_s}},
             {token_not, {token_non_expr, token_non_s}},
             {token_val, {token_non_expr, token_non_s}},
             {token_id, {token_non_expr, token_non_s}},
             {token_left_paren, {token_non_expr, token_non_s}},
             {token_right_brace, {token_eof}},
             {token_eof, {token_eof}}} // end first line content
        },                             // end first line pair
        {// begin second line pair
        token_non_expr,
        {// begin second line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_or, {}},
        {token_and, {}},
        {token_greater_than_equal, {}},
        {token_less_than_equal, {}},
        {token_greater_than, {}},
        {token_less_than, {}},
        {token_equal, {}},
        {token_plus, {}},
        {token_minus, {}},
        {token_multiply, {}},
        {token_divide, {}},
        {token_exponent, {}},
        {token_not, {token_non_expr_lo_or}},
        {token_val, {token_non_expr_lo_or}},
        {token_id, {token_non_expr_lo_or}},
        {token_left_paren, {token_non_expr_lo_or}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_comma, {}},
        {token_eof, {}}
        }// end second line content
        },// end second line pair
        {// begin 3rd line pair
        token_non_expr_lo_or,
        {// begin 3rd line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_or, {}},
        {token_and, {}},
        {token_greater_than_equal, {}},
        {token_less_than_equal, {}},
        {token_greater_than, {}},
        {token_less_than, {}},
        {token_equal, {}},
        {token_plus, {}},
        {token_minus, {}},
        {token_multiply, {}},
        {token_divide, {}},
        {token_exponent, {}},
        {token_not, {token_non_expr_lo_and, token_non_expr_lo_or_rest}},
        {token_val, {token_non_expr_lo_and, token_non_expr_lo_or_rest}},
        {token_id, {token_non_expr_lo_and, token_non_expr_lo_or_rest}},
        {token_left_paren, {token_non_expr_lo_and, token_non_expr_lo_or_rest}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_comma, {}},
        {token_eof, {}}
        }// end 3rd line content
        },// end 3rd line pair
        {// begin 4th line pair
        token_non_expr_lo_or_rest,
        {// begin 4th line content
        {token_type, {token_eof}},
        {token_get, {token_eof}},
        {token_put, {token_eof}},
        {token_doif, {token_eof}},
        {token_do_it_again, {token_eof}},
        {token_do_it_again_until, {token_eof}},
        {token_or, {token_or, token_non_expr_lo_and, token_non_expr_lo_or_rest}},
        {token_and, {token_eof}},
        {token_greater_than_equal, {token_eof}},
        {token_less_than_equal, {token_eof}},
        {token_greater_than, {token_eof}},
        {token_less_than, {token_eof}},
        {token_equal, {token_eof}},
        {token_plus, {token_eof}},
        {token_minus, {token_eof}},
        {token_multiply, {token_eof}},
        {token_divide, {token_eof}},
        {token_exponent, {token_eof}},
        {token_not, {token_eof}},
        {token_val, {token_eof}},
        {token_id, {token_eof}},
        {token_left_paren, {token_eof}},
        {token_right_paren, {token_eof}},
        {token_right_brace, {token_eof}},
        {token_comma, {token_eof}},
        {token_eof, {token_eof}}
        }// end 4th line content
        },// end 4th line pair
        {// begin 5th line pair
        token_non_expr_lo_and,
        {// begin 5th line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_or, {}},
        {token_and, {}},
        {token_greater_than_equal, {}},
        {token_less_than_equal, {}},
        {token_greater_than, {}},
        {token_less_than, {}},
        {token_equal, {}},
        {token_plus, {}},
        {token_minus, {}},
        {token_multiply, {}},
        {token_divide, {}},
        {token_exponent, {}},
        {token_not, {token_non_expr_comp_ge, token_non_expr_lo_and_rest}},
        {token_val, {token_non_expr_comp_ge, token_non_expr_lo_and_rest}},
        {token_id, {token_non_expr_comp_ge, token_non_expr_lo_and_rest}},
        {token_left_paren, {token_non_expr_comp_ge, token_non_expr_lo_and_rest}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_comma, {}},
        {token_eof, {}}
        }// end 5th line content
        },// end 5th line pair
        {// begin 6th line pair
        token_non_expr_lo_and_rest,
        {// begin 6th line content
        {token_type, {token_eof}},
        {token_get, {token_eof}},
        {token_put, {token_eof}},
        {token_doif, {token_eof}},
        {token_do_it_again, {token_eof}},
        {token_do_it_again_until, {token_eof}},
        {token_or, {token_eof}},
        {token_and, {token_and, token_non_expr_comp_ge, token_non_expr_lo_and_rest}},
        {token_greater_than_equal, {token_eof}},
        {token_less_than_equal, {token_eof}},
        {token_greater_than, {token_eof}},
        {token_less_than, {token_eof}},
        {token_equal, {token_eof}},
        {token_plus, {token_eof}},
        {token_minus, {token_eof}},
        {token_multiply, {token_eof}},
        {token_divide, {token_eof}},
        {token_exponent, {token_eof}},
        {token_not, {token_eof}},
        {token_val, {token_eof}},
        {token_id, {token_eof}},
        {token_left_paren, {token_eof}},
        {token_right_paren, {token_eof}},
        {token_right_brace, {token_eof}},
        {token_comma, {token_eof}},
        {token_eof, {token_eof}}
        }// end 6th line content
        },// end 6th line pair
        {// begin 7th line pair
        token_non_expr_comp_ge,
        {// begin 7th line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_or, {}},
        {token_and, {}},
        {token_greater_than_equal, {}},
        {token_less_than_equal, {}},
        {token_greater_than, {}},
        {token_less_than, {}},
        {token_equal, {}},
        {token_plus, {}},
        {token_minus, {}},
        {token_multiply, {}},
        {token_divide, {}},
        {token_exponent, {}},
        {token_not, {token_non_expr_comp_le, token_non_expr_comp_ge_rest}},
        {token_val, {token_non_expr_comp_le, token_non_expr_comp_ge_rest}},
        {token_id, {token_non_expr_comp_le, token_non_expr_comp_ge_rest}},
        {token_left_paren, {token_non_expr_comp_le, token_non_expr_comp_ge_rest}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_comma, {}},
        {token_eof, {}}
        }// end 7th line content
        },// end 7th line pair
        {// begin 8th line pair
        token_non_expr_comp_ge_rest,
        {// begin 8th line content
        {token_type, {token_eof}},
        {token_get, {token_eof}},
        {token_put, {token_eof}},
        {token_doif, {token_eof}},
        {token_do_it_again, {token_eof}},
        {token_do_it_again_until, {token_eof}},
        {token_or, {token_eof}},
        {token_and, {token_eof}},
        {token_greater_than_equal, {token_greater_than_equal, token_non_expr_comp_le, token_non_expr_comp_ge_rest}},
        {token_less_than_equal, {token_eof}},
        {token_greater_than, {token_eof}},
        {token_less_than, {token_eof}},
        {token_equal, {token_eof}},
        {token_plus, {token_eof}},
        {token_minus, {token_eof}},
        {token_multiply, {token_eof}},
        {token_divide, {token_eof}},
        {token_exponent, {token_eof}},
        {token_not, {token_eof}},
        {token_val, {token_eof}},
        {token_id, {token_eof}},
        {token_left_paren, {token_eof}},
        {token_right_paren, {token_eof}},
        {token_right_brace, {token_eof}},
        {token_comma, {token_eof}},
        {token_eof, {token_eof}}
        }// end 8th line content
        },// end 8th line pair
        {// begin 9th line pair
        token_non_expr_comp_le,
        {// begin 9th line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_or, {}},
        {token_and, {}},
        {token_greater_than_equal, {}},
        {token_less_than_equal, {}},
        {token_greater_than, {}},
        {token_less_than, {}},
        {token_equal, {}},
        {token_plus, {}},
        {token_minus, {}},
        {token_multiply, {}},
        {token_divide, {}},
        {token_exponent, {}},
        {token_not, {token_non_expr_comp_g, token_non_expr_comp_le_rest}},
        {token_val, {token_non_expr_comp_g, token_non_expr_comp_le_rest}},
        {token_id, {token_non_expr_comp_g, token_non_expr_comp_le_rest}},
        {token_left_paren, {token_non_expr_comp_g, token_non_expr_comp_le_rest}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_comma, {}},
        {token_eof, {}}
        }// end 9th line content
        },// end 9th line pair
        {// begin 10th line pair
        token_non_expr_comp_le_rest,
        {// begin 10th line content
        {token_type, {token_eof}},
        {token_get, {token_eof}},
        {token_put, {token_eof}},
        {token_doif, {token_eof}},
        {token_do_it_again, {token_eof}},
        {token_do_it_again_until, {token_eof}},
        {token_or, {token_eof}},
        {token_and, {token_eof}},
        {token_greater_than_equal, {token_eof}},
        {token_less_than_equal, {token_less_than_equal, token_non_expr_comp_g, token_non_expr_comp_le_rest}},
        {token_greater_than, {token_eof}},
        {token_less_than, {token_eof}},
        {token_equal, {token_eof}},
        {token_plus, {token_eof}},
        {token_minus, {token_eof}},
        {token_multiply, {token_eof}},
        {token_divide, {token_eof}},
        {token_exponent, {token_eof}},
        {token_not, {token_eof}},
        {token_val, {token_eof}},
        {token_id, {token_eof}},
        {token_left_paren, {token_eof}},
        {token_right_paren, {token_eof}},
        {token_right_brace, {token_eof}},
        {token_comma, {token_eof}},
        {token_eof, {token_eof}}
        }// end 10th line content
        },// end 10th line pair
        {// begin 11st line pair
        token_non_expr_comp_g,
        {// begin 11st line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_or, {}},
        {token_and, {}},
        {token_greater_than_equal, {}},
        {token_less_than_equal, {}},
        {token_greater_than, {}},
        {token_less_than, {}},
        {token_equal, {}},
        {token_plus, {}},
        {token_minus, {}},
        {token_multiply, {}},
        {token_divide, {}},
        {token_exponent, {}},
        {token_not, {token_non_expr_comp_l, token_non_expr_comp_g_rest}},
        {token_val, {token_non_expr_comp_l, token_non_expr_comp_g_rest}},
        {token_id, {token_non_expr_comp_l, token_non_expr_comp_g_rest}},
        {token_left_paren, {token_non_expr_comp_l, token_non_expr_comp_g_rest}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_comma, {}},
        {token_eof, {}}
        }// end 11st line content
        },// end 11st line pair
        {// begin 12nd line pair
        token_non_expr_comp_g_rest,
        {// begin 12nd line content
        {token_type, {token_eof}},
        {token_get, {token_eof}},
        {token_put, {token_eof}},
        {token_doif, {token_eof}},
        {token_do_it_again, {token_eof}},
        {token_do_it_again_until, {token_eof}},
        {token_or, {token_eof}},
        {token_and, {token_eof}},
        {token_greater_than_equal, {token_eof}},
        {token_less_than_equal, {token_eof}},
        {token_greater_than, {token_greater_than, token_non_expr_comp_l, token_non_expr_comp_g_rest}},
        {token_less_than, {token_eof}},
        {token_equal, {token_eof}},
        {token_plus, {token_eof}},
        {token_minus, {token_eof}},
        {token_multiply, {token_eof}},
        {token_divide, {token_eof}},
        {token_exponent, {token_eof}},
        {token_not, {token_eof}},
        {token_val, {token_eof}},
        {token_id, {token_eof}},
        {token_left_paren, {token_eof}},
        {token_right_paren, {token_eof}},
        {token_right_brace, {token_eof}},
        {token_comma, {token_eof}},
        {token_eof, {token_eof}}
        }// end 12nd line content
        },// end 12nd line pair
        {// begin 13rd line pair
        token_non_expr_comp_l,
        {// begin 13rd line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_or, {}},
        {token_and, {}},
        {token_greater_than_equal, {}},
        {token_less_than_equal, {}},
        {token_greater_than, {}},
        {token_less_than, {}},
        {token_equal, {}},
        {token_plus, {}},
        {token_minus, {}},
        {token_multiply, {}},
        {token_divide, {}},
        {token_exponent, {}},
        {token_not, {token_non_expr_comp_e, token_non_expr_comp_l_rest}},
        {token_val, {token_non_expr_comp_e, token_non_expr_comp_l_rest}},
        {token_id, {token_non_expr_comp_e, token_non_expr_comp_l_rest}},
        {token_left_paren, {token_non_expr_comp_e, token_non_expr_comp_l_rest}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_comma, {}},
        {token_eof, {}}
        }// end 13rd line content
        },// end 13rd line pair
        {// begin 14th line pair
        token_non_expr_comp_l_rest,
        {// begin 14th line content
        {token_type, {token_eof}},
        {token_get, {token_eof}},
        {token_put, {token_eof}},
        {token_doif, {token_eof}},
        {token_do_it_again, {token_eof}},
        {token_do_it_again_until, {token_eof}},
        {token_or, {token_eof}},
        {token_and, {token_eof}},
        {token_greater_than_equal, {token_eof}},
        {token_less_than_equal, {token_eof}},
        {token_greater_than, {token_eof}},
        {token_less_than, {token_less_than, token_non_expr_comp_e, token_non_expr_comp_l_rest}},
        {token_equal, {token_eof}},
        {token_plus, {token_eof}},
        {token_minus, {token_eof}},
        {token_multiply, {token_eof}},
        {token_divide, {token_eof}},
        {token_exponent, {token_eof}},
        {token_not, {token_eof}},
        {token_val, {token_eof}},
        {token_id, {token_eof}},
        {token_left_paren, {token_eof}},
        {token_right_paren, {token_eof}},
        {token_right_brace, {token_eof}},
        {token_comma, {token_eof}},
        {token_eof, {token_eof}}
        }// end 14th line content
        },// end 14th line pair
        {// begin 15th line pair
        token_non_expr_comp_e,
        {// begin 15th line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_or, {}},
        {token_and, {}},
        {token_greater_than_equal, {}},
        {token_less_than_equal, {}},
        {token_greater_than, {}},
        {token_less_than, {}},
        {token_equal, {}},
        {token_plus, {}},
        {token_minus, {}},
        {token_multiply, {}},
        {token_divide, {}},
        {token_exponent, {}},
        {token_not, {token_non_expr_arit_as, token_non_expr_comp_e_rest}},
        {token_val, {token_non_expr_arit_as, token_non_expr_comp_e_rest}},
        {token_id, {token_non_expr_arit_as, token_non_expr_comp_e_rest}},
        {token_left_paren, {token_non_expr_arit_as, token_non_expr_comp_e_rest}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_comma, {}},
        {token_eof, {}}
        }// end 15th line content
        },// end 15th line pair
        {// begin 16th line pair
        token_non_expr_comp_e_rest,
        {// begin 16th line content
        {token_type, {token_eof}},
        {token_get, {token_eof}},
        {token_put, {token_eof}},
        {token_doif, {token_eof}},
        {token_do_it_again, {token_eof}},
        {token_do_it_again_until, {token_eof}},
        {token_or, {token_eof}},
        {token_and, {token_eof}},
        {token_greater_than_equal, {token_eof}},
        {token_less_than_equal, {token_eof}},
        {token_greater_than, {token_eof}},
        {token_less_than, {token_eof}},
        {token_equal, {token_equal, token_non_expr_arit_as, token_non_expr_comp_e_rest}},
        {token_plus, {token_eof}},
        {token_minus, {token_eof}},
        {token_multiply, {token_eof}},
        {token_divide, {token_eof}},
        {token_exponent, {token_eof}},
        {token_not, {token_eof}},
        {token_val, {token_eof}},
        {token_id, {token_eof}},
        {token_left_paren, {token_eof}},
        {token_right_paren, {token_eof}},
        {token_right_brace, {token_eof}},
        {token_comma, {token_eof}},
        {token_eof, {token_eof}}
        }// end 16th line content
        },// end 16th line pair
        {// begin 17th line pair
        token_non_expr_arit_as,
        {// begin 17th line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_or, {}},
        {token_and, {}},
        {token_greater_than_equal, {}},
        {token_less_than_equal, {}},
        {token_greater_than, {}},
        {token_less_than, {}},
        {token_equal, {}},
        {token_plus, {}},
        {token_minus, {}},
        {token_multiply, {}},
        {token_divide, {}},
        {token_exponent, {}},
        {token_not, {token_non_expr_arit_md, token_non_expr_arit_as_rest}},
        {token_val, {token_non_expr_arit_md, token_non_expr_arit_as_rest}},
        {token_id, {token_non_expr_arit_md, token_non_expr_arit_as_rest}},
        {token_left_paren, {token_non_expr_arit_md, token_non_expr_arit_as_rest}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_comma, {}},
        {token_eof, {}}
        }// end 17th line content
        },// end 17th line pair
        {// begin 18th line pair
        token_non_expr_arit_as_rest,
        {// begin 18th line content
        {token_type, {token_eof}},
        {token_get, {token_eof}},
        {token_put, {token_eof}},
        {token_doif, {token_eof}},
        {token_do_it_again, {token_eof}},
        {token_do_it_again_until, {token_eof}},
        {token_or, {token_eof}},
        {token_and, {token_eof}},
        {token_greater_than_equal, {token_eof}},
        {token_less_than_equal, {token_eof}},
        {token_greater_than, {token_eof}},
        {token_less_than, {token_eof}},
        {token_equal, {token_eof}},
        {token_plus, {token_plus, token_non_expr_arit_md, token_non_expr_arit_as_rest}},
        {token_minus, {token_minus, token_non_expr_arit_md, token_non_expr_arit_as_rest}},
        {token_multiply, {token_eof}},
        {token_divide, {token_eof}},
        {token_exponent, {token_eof}},
        {token_not, {token_eof}},
        {token_val, {token_eof}},
        {token_id, {token_eof}},
        {token_left_paren, {token_eof}},
        {token_right_paren, {token_eof}},
        {token_right_brace, {token_eof}},
        {token_comma, {token_eof}},
        {token_eof, {token_eof}}
        }// end 18th line content
        },// end 18th line pair
        {// begin 19th line pair
        token_non_expr_arit_md,
        {// begin 19th line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_or, {}},
        {token_and, {}},
        {token_greater_than_equal, {}},
        {token_less_than_equal, {}},
        {token_greater_than, {}},
        {token_less_than, {}},
        {token_equal, {}},
        {token_plus, {}},
        {token_minus, {}},
        {token_multiply, {}},
        {token_divide, {}},
        {token_exponent, {}},
        {token_not, {token_non_expr_arit_exp, token_non_expr_arit_md_rest}},
        {token_val, {token_non_expr_arit_exp, token_non_expr_arit_md_rest}},
        {token_id, {token_non_expr_arit_exp, token_non_expr_arit_md_rest}},
        {token_left_paren, {token_non_expr_arit_exp, token_non_expr_arit_md_rest}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_comma, {}},
        {token_eof, {}}
        }// end 19th line content
        },// end 19th line pair
        {// begin 20th line pair
        token_non_expr_arit_md_rest,
        {// begin 20th line content
        {token_type, {token_eof}},
        {token_get, {token_eof}},
        {token_put, {token_eof}},
        {token_doif, {token_eof}},
        {token_do_it_again, {token_eof}},
        {token_do_it_again_until, {token_eof}},
        {token_or, {token_eof}},
        {token_and, {token_eof}},
        {token_greater_than_equal, {token_eof}},
        {token_less_than_equal, {token_eof}},
        {token_greater_than, {token_eof}},
        {token_less_than, {token_eof}},
        {token_equal, {token_eof}},
        {token_plus, {token_eof}},
        {token_minus, {token_eof}},
        {token_multiply, {token_multiply, token_non_expr_arit_exp, token_non_expr_arit_md_rest}},
        {token_divide, {token_divide, token_non_expr_arit_exp, token_non_expr_arit_md_rest}},
        {token_exponent, {token_eof}},
        {token_not, {token_eof}},
        {token_val, {token_eof}},
        {token_id, {token_eof}},
        {token_left_paren, {token_eof}},
        {token_right_paren, {token_eof}},
        {token_right_brace, {token_eof}},
        {token_comma, {token_eof}},
        {token_eof, {token_eof}}
        }// end 20th line content
        },// end 20th line pair
        {// begin 21st line pair
        token_non_expr_arit_exp,
        {// begin 21st line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_or, {}},
        {token_and, {}},
        {token_greater_than_equal, {}},
        {token_less_than_equal, {}},
        {token_greater_than, {}},
        {token_less_than, {}},
        {token_equal, {}},
        {token_plus, {}},
        {token_minus, {}},
        {token_multiply, {}},
        {token_divide, {}},
        {token_exponent, {}},
        {token_not, {token_non_expr_not, token_non_expr_arit_exp_rest}},
        {token_val, {token_non_expr_not, token_non_expr_arit_exp_rest}},
        {token_id, {token_non_expr_not, token_non_expr_arit_exp_rest}},
        {token_left_paren, {token_non_expr_not, token_non_expr_arit_exp_rest}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_comma, {}},
        {token_eof, {}}
        }// end 21st line content
        },// end 21st line pair
        {// begin 22nd line pair
        token_non_expr_arit_exp_rest,
        {// begin 22nd line content
        {token_type, {token_eof}},
        {token_get, {token_eof}},
        {token_put, {token_eof}},
        {token_doif, {token_eof}},
        {token_do_it_again, {token_eof}},
        {token_do_it_again_until, {token_eof}},
        {token_or, {token_eof}},
        {token_and, {token_eof}},
        {token_greater_than_equal, {token_eof}},
        {token_less_than_equal, {token_eof}},
        {token_greater_than, {token_eof}},
        {token_less_than, {token_eof}},
        {token_equal, {token_eof}},
        {token_plus, {token_eof}},
        {token_minus, {token_eof}},
        {token_multiply, {token_eof}},
        {token_divide, {token_eof}},
        {token_exponent, {token_exponent, token_non_expr_arit_exp}},
        {token_not, {token_eof}},
        {token_val, {token_eof}},
        {token_id, {token_eof}},
        {token_left_paren, {token_eof}},
        {token_right_paren, {token_eof}},
        {token_right_brace, {token_eof}},
        {token_comma, {token_eof}},
        {token_eof, {token_eof}}
        }// end 22nd line content
        },// end 22nd line pair
        {// begin 23rd line pair
        token_non_expr_not,
        {// begin 23rd line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_or, {}},
        {token_and, {}},
        {token_greater_than_equal, {}},
        {token_less_than_equal, {}},
        {token_greater_than, {}},
        {token_less_than, {}},
        {token_equal, {}},
        {token_plus, {}},
        {token_minus, {}},
        {token_multiply, {}},
        {token_divide, {}},
        {token_exponent, {}},
        {token_not, {token_not, token_non_expr_not}},
        {token_val, {token_non_expr_final}},
        {token_id, {token_non_expr_final}},
        {token_left_paren, {token_non_expr_final}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_comma, {}},
        {token_eof, {}}
        }// end 23rd line content
        },// end 23rd line pair
        {// begin 24th line pair
        token_non_expr_final,
        {// begin 24th line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_or, {}},
        {token_and, {}},
        {token_greater_than_equal, {}},
        {token_less_than_equal, {}},
        {token_greater_than, {}},
        {token_less_than, {}},
        {token_equal, {}},
        {token_plus, {}},
        {token_minus, {}},
        {token_multiply, {}},
        {token_divide, {}},
        {token_exponent, {}},
        {token_not, {}},
        {token_val, {token_val}},
        {token_id, {token_id, token_non_expr_final_rest}},
        {token_left_paren, {token_left_paren, token_non_expr, token_right_paren}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_comma, {}},
        {token_eof, {}}
        }// end 24th line content
        },// end 24th line pair
        {// begin 25th line pair
        token_non_expr_final_rest,
        {// begin 25th line content
        {token_type, {token_eof}},
        {token_get, {token_eof}},
        {token_put, {token_eof}},
        {token_doif, {token_eof}},
        {token_do_it_again, {token_eof}},
        {token_do_it_again_until, {token_eof}},
        {token_or, {token_eof}},
        {token_and, {token_eof}},
        {token_greater_than_equal, {token_eof}},
        {token_less_than_equal, {token_eof}},
        {token_greater_than, {token_eof}},
        {token_less_than, {token_eof}},
        {token_equal, {token_eof}},
        {token_plus, {token_eof}},
        {token_minus, {token_eof}},
        {token_multiply, {token_eof}},
        {token_divide, {token_eof}},
        {token_exponent, {token_eof}},
        {token_not, {token_eof}},
        {token_val, {token_eof}},
        {token_id, {token_eof}},
        {token_left_paren, {token_eof}},
        {token_right_paren, {token_eof}},
        {token_assign, {token_assign, token_non_expr}},
        {token_right_brace, {token_eof}},
        {token_comma, {token_eof}},
        {token_eof, {token_eof}}
        }// end 25th line content
        },// end 25th line pair
        {//begin 26th line pair
            token_non_typedecl,
{// begin 26th line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_not, {}},
        {token_val, {}},
        {token_id, {token_id, token_non_typedecl_attrib}},
        {token_left_paren, {}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_comma, {}},
        {token_eof, {}}
}// end 26th line content
},//end 26th line pair
{// begin 27th line pair
        token_non_typedecl_attrib,
        {// begin 27th line content
        {token_type, {token_eof}},
        {token_get, {token_eof}},
        {token_put, {token_eof}},
        {token_doif, {token_eof}},
        {token_do_it_again, {token_eof}},
        {token_do_it_again_until, {token_eof}},
        {token_not, {token_eof}},
        {token_val, {token_eof}},
        {token_id, {token_eof}},
        {token_left_paren, {token_eof}},
        {token_right_paren, {}},
        {token_assign, {token_assign, token_non_expr}},
        {token_right_brace, {token_eof}},
        {token_eof, {token_eof}}
        }// end 27th line content
        },// end 27th line pair
                {//begin 28th line pair
            token_non_getfunc,
{// begin 28th line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_not, {}},
        {token_val, {}},
        {token_id, {}},
        {token_left_paren, {token_left_paren, token_id, token_right_paren}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_eof, {}}
}// end 28th line content
},//end 28th line pair
{//begin 29th line pair
            token_non_putfunc,
{// begin 29th line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_not, {}},
        {token_val, {}},
        {token_id, {}},
        {token_left_paren, {token_left_paren, token_non_expr, token_right_paren}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_eof, {}}
}// end 29th line content
},//end 29th line pair
{//begin 30th line pair
            token_non_doif,
{// begin 30th line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_not, {}},
        {token_val, {}},
        {token_id, {}},
        {token_left_paren, {token_left_paren, token_non_expr, token_right_paren, token_left_brace, token_non_s, token_right_brace, token_non_maybe_not}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_eof, {}}
}// end 30th line content
},//end 30th line pair
{// begin 31st line pair
        token_non_maybe_not,
        {// begin 31st line content
        {token_type, {token_eof}},
        {token_get, {token_eof}},
        {token_put, {token_eof}},
        {token_doif, {token_eof}},
        {token_do_it_again, {token_eof}},
        {token_do_it_again_until, {token_eof}},
        {token_maybe_not, {token_maybe_not, token_left_brace, token_non_s, token_right_brace}},
        {token_not, {token_eof}},
        {token_val, {token_eof}},
        {token_id, {token_eof}},
        {token_left_paren, {token_eof}},
        {token_right_paren, {token_eof}},
        {token_right_brace, {token_eof}},
        {token_eof, {token_eof}}
        }// end 31st line content
        },// end 31stth line pair
        {//begin 32nd line pair
            token_non_do_it_again,
{// begin 32nd line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_not, {}},
        {token_val, {}},
        {token_id, {}},
        {token_left_paren, {token_left_paren, token_non_expr, token_right_paren, token_left_brace, token_non_s, token_right_brace}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_eof, {}}
}// end 32nd line content
},//end 32nd line pair
        {//begin 33rd line pair
            token_non_do_it_again_until,
{// begin 33rd line content
        {token_type, {}},
        {token_get, {}},
        {token_put, {}},
        {token_doif, {}},
        {token_do_it_again, {}},
        {token_do_it_again_until, {}},
        {token_not, {}},
        {token_val, {}},
        {token_id, {}},
        {token_left_paren, {token_left_paren, token_id, token_comma, token_non_expr, token_comma, token_non_expr, token_comma, token_non_expr, token_right_paren, token_left_brace, token_non_s, token_right_brace}},
        {token_right_paren, {}},
        {token_right_brace, {}},
        {token_eof, {}}
}// end 33rd line content
}//end 33rd line pair
}; // end main hash map