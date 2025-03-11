#pragma once
#include <unordered_map>
#include <list>
#include <vector>
#include "lexer_tokens.h"
void lr_parse(const std::vector<token> &tokens, bool &result);
extern std::unordered_map<token_class, std::unordered_map<token_class, std::list<token_class>>> lr_table;