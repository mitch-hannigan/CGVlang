#pragma once
#include <unordered_map>
#include <list>
#include <vector>
#include "lexer_tokens.h"
std::unordered_map<token_class, std::unordered_map<token_class, std::list<token_class>>> lr_table =
    {
        {token_non_s, {
                          {token_type, {token_type, token_non_typedecl, token_non_s}},
                          {token_get, {token_get, token_non_getfunc, token_non_s}},
                          {token_put, {token_put, token_non_putfunc, token_non_s}},
                          {token_doif, {token_doif, token_non_doif, token_non_s}},
                          {token_do_it_again, {token_do_it_again, token_non_do_it_again, token_non_s}},
                          {token_do_it_again_until, {token_do_it_again_until, token_non_do_it_again_until, token_non_s}},
                      }}};
void lr_parse(const std::vector<token> &tokens, bool &result);