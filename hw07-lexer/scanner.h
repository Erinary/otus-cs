//
// Created by Liubov Udalova on 01.05.25.
//

#ifndef SCANNER_H
#define SCANNER_H
#include <stdbool.h>

#include "list.h"

typedef enum {
    // Single-character tokens.
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN, TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS, TOKEN_SEMICOLON,
    TOKEN_SLASH, TOKEN_STAR,
    // Custom token
    TOKEN_POWER,

    // One or two character tokens.
    TOKEN_BANG, TOKEN_BANG_EQUAL, TOKEN_EQUAL, TOKEN_EQUAL_EQUAL, TOKEN_GREATER,
    TOKEN_GREATER_EQUAL, TOKEN_LESS, TOKEN_LESS_EQUAL,
    // Custom tokens
    TOKEN_BIT_WISE_AND, TOKEN_BIT_WISE_OR, TOKEN_LOGICAL_AND, TOKEN_LOGICAL_OR,

    // Literals
    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
    // Keywords
    TOKEN_AND, TOKEN_ELSE, TOKEN_FALSE, TOKEN_FOR, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
    TOKEN_PRINT, TOKEN_RETURN, TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,

    TOKEN_ERROR, TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    const char *start;
    int length;
    int line;
} Token;


void initScanner(const char *source);

Token scanToken();

List *scanTokens(const char *source, bool log_results);

#endif //SCANNER_H
