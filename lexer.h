#pragma once
#include <vector>
#include <string>
#include "lexer_tokens.h"
#include "lexer_keywords.h"
std::vector<token> lex_analyze(const std::string &source, bool &result);