#pragma once
#include <string>
#include "lexer_tokens.h"
std::string get_tac_text_form(const token &tok);
std::string get_tac_operator(token_class tclass);
std::string get_tac_type(token_class tok);
std::string generate_tac_cast_code(token_class tclass);