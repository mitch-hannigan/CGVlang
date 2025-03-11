#pragma once
#include <string>
#include "semantic.h"
#include <unordered_map>
#include <list>
#include <vector>
#include "lexer_tokens.h"
std::string ll_parse(const std::vector<token> &tokens, bool &result);
extern std::unordered_map<token_class, std::unordered_map<token_class, std::pair<std::list<token_class>, semantic_rule>>> ll_table;