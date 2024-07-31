#pragma once
#include <list>
#include <string>
#include "lexer_tokens.h"
#include "lexer_keywords.h"
std::list<token> lex_analyze(const std::string &source, bool &result);