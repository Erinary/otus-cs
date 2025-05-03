%{
#include "../ast.h"
#include "parser.h"
%}

%%

[a-zA-Z_][a-zA-Z0-9_]* { yylval.value_str = strdup(yytext); return IDENT; }
[0-9]+      { yylval.value_number = atoi(yytext); return NUMBER; }
"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MUL; }
"/"         { return DIV; }
"("         { return PARENTHESIS_LEFT; }
")"         { return PARENTHESIS_RIGHT; }
"="         { return ASSIGN; }
[ \t\n]     { /* skip whitespace */ }
.           { return yytext[0]; }

%%

// Crunch to not depend on Flex support library (magic trick from StackOverflow):
int yywrap(void) {
    return 1;
}
