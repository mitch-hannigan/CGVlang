#include "lexer_keywords.h"
std::unordered_map<std::string, token_class> keywords = {
    {"get", token_get},
    {"put", token_put},
    {"doif", token_doif},
    {"maybe_not", token_maybe_not},
    {"do_it_again", token_do_it_again},
    {"do_it_again_until", token_do_it_again_until},
    {"byte", token_type_byte},
    {"int", token_type_int},
    {"real", token_type_real},
    {"and", token_and},
    {"or", token_or},
    {"not", token_not}
};