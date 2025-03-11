#pragma once
#include "semantic.h"
#include <unordered_map>
#include <list>
#include <vector>
#include "lexer_tokens.h"
void ll_parse(const std::vector<token> &tokens, bool &result);
extern std::unordered_map<token_class, std::unordered_map<token_class, std::pair<std::list<token_class>, semantic_rule>>> ll_table;