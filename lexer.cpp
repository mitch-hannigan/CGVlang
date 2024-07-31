#include "lexer.h"
#include <list>
#include <string>
#include <cctype>
#include <iostream>
inline void lex_process_newline(size_t &line, size_t &col)
{
    line++;
    col = 1; // not 0 based
}
inline void lex_process_comment(const std::string &source, size_t &i, size_t &length)
{
    while (i < length && source[i] != '\n')
        ++i;
    --i; // backtrack to handle '\n'
}
inline void lex_process_number(const std::string &source, size_t length, token &tok, size_t &i, size_t &col)
{
    tok.tclass = token_val_int; // Assume inicialmente que é um int
    while (i < length && (std::isdigit(source[i]) || source[i] == '.') || source[i] == 'b')
    {
        if (tok.tclass == token_val_byte)
        {
            tok.tclass = token_error;
            tok.text += std::string(" expect a witespace after a byteval, not ") + source[i] + std::string(".");
            break;
        }
        if (source[i] == '.')
        {
            if (tok.tclass == token_val_real)
            {
                tok.tclass = token_error;
                tok.text += " can't be a real twice.";
                break;
            }
            tok.tclass = token_val_real; // Atualiza para real se houver um ponto
        }
        if (source[i] == 'b')
        {
            if (tok.tclass == token_val_real)
            {
                tok.tclass = token_error;
                tok.text += " we don't allow explicit b conversion from real.";
                break;
            }
            tok.tclass = token_val_byte; // Atualiza para byte se houver um b
        }
        tok.text += source[i];
        i++;
        col++;
    }
    --i, --col; // backtrack to handle the last character
}
inline void lex_check_keyword(token &tok)
{
    auto it = keywords.find(tok.text);
    if (it != keywords.end())
        tok.tclass = it->second;
    else
        tok.tclass = token_id;
}
inline void lex_process_id(const std::string &source, size_t length, token &tok, size_t &i, size_t &col)
{
    while (i < length && (std::isalnum(source[i]) || source[i] == '_'))
    {
        tok.text += source[i];
        i++, col++;
    }
    --i, --col; // backtrack to handle the last character
    lex_check_keyword(tok);
}
inline bool lex_is_arithmetic_operator(const std::string &source, size_t length, char c, token &tok, size_t &i, size_t &col)
{
    switch (c)
    {
    case '+':
        tok.tclass = token_plus;
        tok.text = "+";
        return true;
    case '-':
        tok.tclass = token_minus;
        tok.text = "-";
        return true;
    case '*':
        tok.tclass = token_multiply;
        tok.text = "*";
        if (i + 1 < length && source[i + 1] == '*')
        {
            tok.tclass = token_exponent;
            tok.text = "**";
            ++i;
            ++col;
        }
        return true;
    case '/':
        tok.tclass = token_divide;
        tok.text = "/";
        return true;
    default:
        return false;
    }
}
inline bool lex_is_comparison_operator(const std::string &source, size_t length, char c, token &tok, size_t &i, size_t &col)
{
    switch (c)
    {
    case '=':
        tok.tclass = token_assign;
        tok.text = "=";
        if (i + 1 < length && source[i + 1] == '=')
        {
            tok.tclass = token_equal;
            tok.text = "==";
            ++i;
            ++col;
        }
        return true;
    case '<':
        tok.tclass = token_less_than;
        tok.text = "<";
        if (i + 1 < length && source[i + 1] == '=')
        {
            tok.tclass = token_less_than_equal;
            tok.text = "<=";
            ++i;
            ++col;
        }
        return true;
    case '>':
        tok.tclass = token_greater_than;
        tok.text = ">";
        if (i + 1 < length && source[i + 1] == '=')
        {
            tok.tclass = token_greater_than_equal;
            tok.text = ">=";
            ++i;
            ++col;
        }
        return true;
    default:
        return false;
    }
}
inline bool lex_is_marker(const std::string &source, size_t length, char c, token &tok, size_t &i, size_t &col)
{
    switch (c)
    {
    case '(':
        tok.tclass = token_left_paren;
        tok.text = "(";
        return true;
    case ')':
        tok.tclass = token_right_paren;
        tok.text = ")";
        return true;
    case '{':
        tok.tclass = token_left_brace;
        tok.text = "{";
        return true;
    case '}':
        tok.tclass = token_right_brace;
        tok.text = "}";
        return true;
    case ',':
        tok.tclass = token_comma;
        tok.text = ",";
        return true;
    default:
        return false;
    }
}
inline void lex_is_single_char(const std::string &source, size_t length, char c, token &tok, size_t &i, size_t &col)
{
    if (!(lex_is_arithmetic_operator(source, length, c, tok, i, col) || lex_is_comparison_operator(source, length, c, tok, i, col) || lex_is_marker(source, length, c, tok, i, col)))
    {
        tok.tclass = token_error;
        tok.text = std::string("Unknown character ") + c + std::string(".");
    }
}
std::list<token> lex_analyze(const std::string &source, bool &result)
{
    std::list<token> tokens;
    size_t length = source.length();
    size_t line = 1, col = 1;
    result = true;
    for (size_t i = 0; i < length; ++i, ++col)
    {
        char c = source[i];
        if (std::isspace(c)) // witespace test
        {
            if (c == '\n')
                lex_process_newline(line, col);
            continue;
        }
        if (c == '#')
        {
            lex_process_comment(source, i, length);
            continue;
        }
        token tok;
        tok.line = line;
        tok.col = col;
        if (std::isdigit(c))
        {
            lex_process_number(source, length, tok, i, col);
            result = result && tok.tclass != token_error;
            tokens.push_back(tok);
            continue;
        }
        if (std::isalpha(c) || c == '_')
        {
            lex_process_id(source, length, tok, i, col);
            result = result && tok.tclass != token_error;
            tokens.push_back(tok);
            continue;
        }
        lex_is_single_char(source, length, c, tok, i, col);
        result = result && tok.tclass != token_error;
        tokens.push_back(tok);
    }
    return tokens;
}