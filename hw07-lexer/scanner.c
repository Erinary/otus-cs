//
// Created by Liubov Udalova on 01.05.25.
//

#include "scanner.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "list.h"

typedef struct {
    const char *start;
    const char *current;
    int line;
    List lexems;
} Scanner;

Scanner scanner;

// Инициализация сканера
void initScanner(const char *source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
    scanner.lexems.head = NULL;
}

// Вспомогательные функции определения символа, цифры, окончания строки
// Функция is-alpha
static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

// Функция is-digit
static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

// Определения конца строки
static bool isAtEnd() {
    return *scanner.current == '\0';
}

//> advance
static char advance() {
    scanner.current++;
    return scanner.current[-1];
}

//> peek
static char peek() {
    return *scanner.current;
}

//> peek-next
static char peekNext() {
    if (isAtEnd()) {
        return '\0';
    }
    return scanner.current[1];
}

//> match
static bool match(char expected) {
    if (isAtEnd()) {
        return false;
    }
    if (*scanner.current != expected) {
        return false;
    }
    scanner.current++;
    return true;
}

//> make-token
static Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int) (scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}

//> error-token
static Token errorToken(const char *message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int) strlen(message);
    token.line = scanner.line;
    return token;
}

//> skip-whitespace
static void skipWhitespace() {
    while (1) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            //> newline
            case '\n':
                scanner.line++;
                advance();
                break;
            //> comment
            case '/':
                if (peekNext() == '/') {
                    // A comment goes until the end of the line.
                    while (peek() != '\n' && !isAtEnd()) {
                        advance();
                    }
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}


//> check-keyword
static TokenType checkKeyword(int start, int length, const char *rest, TokenType type) {
    if (scanner.current - scanner.start == start + length &&
        memcmp(scanner.start + start, rest, length) == 0) {
        return type;
    }
    return TOKEN_IDENTIFIER;
}

//> identifier-type
static TokenType identifierType() {
    //> keywords
    switch (scanner.start[0]) {
        case 'a':
            return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'e':
            return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        case 'f':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'a':
                        return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o':
                        return checkKeyword(2, 1, "r", TOKEN_FOR);
                    default:
                        break;
                }
            }
            break;
        case 'i':
            return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'n':
            return checkKeyword(1, 2, "il", TOKEN_NIL);
        case 'o':
            return checkKeyword(1, 1, "r", TOKEN_OR);
        case 'p':
            return checkKeyword(1, 4, "rint", TOKEN_PRINT);
        case 'r':
            return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        case 't':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'r':
                        return checkKeyword(2, 2, "ue", TOKEN_TRUE);
                    default:
                        break;
                }
            }
            break;
        case 'v':
            return checkKeyword(1, 2, "ar", TOKEN_VAR);
        case 'w':
            return checkKeyword(1, 4, "hile", TOKEN_WHILE);
        default:
            break;
    }
    //< keywords
    return TOKEN_IDENTIFIER;
}

//> identifier
static Token identifier() {
    while (isAlpha(peek()) || isDigit(peek())) {
        advance();
    }
    return makeToken(identifierType());
}

//> number
static Token number() {
    while (isDigit(peek())) {
        advance();
    }
    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the ".".
        advance();
        while (isDigit(peek())) {
            advance();
        }
    }
    return makeToken(TOKEN_NUMBER);
}

//> string
static Token string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            scanner.line++;
        }
        advance();
    }
    if (isAtEnd()) {
        return errorToken("Unterminated string.");
    }
    // The closing quote.
    advance();
    return makeToken(TOKEN_STRING);
}

Token scanToken() {
    skipWhitespace();
    scanner.start = scanner.current;
    if (isAtEnd()) {
        return makeToken(TOKEN_EOF);
    }
    const char c = advance();
    if (isAlpha(c)) {
        return identifier();
    }
    if (isDigit(c)) {
        return number();
    }

    switch (c) {
        case '(':
            return makeToken(TOKEN_LEFT_PAREN);
        case ')':
            return makeToken(TOKEN_RIGHT_PAREN);
        case '{':
            return makeToken(TOKEN_LEFT_BRACE);
        case '}':
            return makeToken(TOKEN_RIGHT_BRACE);
        case ';':
            return makeToken(TOKEN_SEMICOLON);
        case ',':
            return makeToken(TOKEN_COMMA);
        case '.':
            return makeToken(TOKEN_DOT);
        case '-':
            return makeToken(TOKEN_MINUS);
        case '+':
            return makeToken(TOKEN_PLUS);
        case '/':
            return makeToken(TOKEN_SLASH);
        case '*':
            return makeToken(TOKEN_STAR);
        case '^':
            return makeToken(TOKEN_POWER);
        case '!':
            return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '&':
            return makeToken(match('&') ? TOKEN_LOGICAL_AND : TOKEN_AND);
        case '|':
            return makeToken(match('|') ? TOKEN_LOGICAL_OR : TOKEN_OR);
        case '"':
            return string();
        default:
            return errorToken("Unexpected character.");
    }
}

List *scanTokens(const char *source, const bool log_results) {
    initScanner(source);
    int line = -1;
    while (1) {
        Token token = scanToken();
        scanner.start = scanner.current;

        if (token.line != line) {
            if (log_results) {
                printf("%4d ", token.line);
            }
            line = token.line;
        } else {
            if (log_results) {
                printf("   | ");
            }
        }
        if (log_results) {
            printf("%2d '%.*s'\n", token.type, token.length, token.start); // [format]
        }
        Node *nd = create_node((void *) &token);
        scanner.lexems.head = add_last(scanner.lexems.head, nd);
        if (token.type == TOKEN_EOF) break;
    }
    return &scanner.lexems;
}
