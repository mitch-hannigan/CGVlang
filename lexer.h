#pragma once
#include <list>
#include <string>
#include "lexer_tokens.h"
std::list<token> lex_analyze(std::string text);