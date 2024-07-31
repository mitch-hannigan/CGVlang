#pragma once
#include <string>
#include <unordered_map>
#include "lexer_tokens.h"
std::unordered_map<std::string, token_class> keywords = {
    {"get", token_get},
    {"put", token_put},
    {"doif", token_doif},
    {"maybe_not", token_maybe_not},
    {"do_it_again", token_do_it_again},
    {"byte", token_type_byte},
    {"int", token_type_int},
    {"real", token_type_real},
    {"and", token_and},
    {"or", token_or},
    {"not", token_not}
};