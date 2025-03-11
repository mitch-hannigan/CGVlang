#pragma once
#include "lexer_tokens.h"
#include <string>
#include <sstream>
#include <unordered_map>
#include <list>
std::string tokenClassToString(token_class tclass);
std::string get_expected_tokens(const std::unordered_map<token_class, std::list<token_class>> &line);
token_class t_class_basic(token_class tok);