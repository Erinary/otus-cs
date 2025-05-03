%{
#include "../ast.h"
#include "parser.h"
%}

%%

[0-9]+      { yylval.value_number = atoi(yytext); return NUMBER; }
"+"         { return PLUS; }
"-"         { return MINUS; }
[ \t\n]     { /* skip whitespace */ }
.           { return yytext[0]; }

%%

// Crunch to not depend on Flex support library (magic trick from StackOverflow):
int yywrap(void) {
    return 1;
}
