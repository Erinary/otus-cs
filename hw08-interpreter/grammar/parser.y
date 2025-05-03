%{
#include "../ast.h"
#include "lexer.h"

int yyerror(const char *s) {
    fprintf(stderr, "Parsing error, cleaning singleton_ast AST... : %s\n", s);
    free_ast(singleton_ast);
    singleton_ast = NULL;
    return 0;
}
%}

%union {
    float value_number;
    AST* node;
}

%token <value_number> NUMBER
%token <node> ID
%token PLUS MINUS MUL DIV
%type <node> expr

%start input

%left PLUS MINUS
%left MUL DIV

%%

input:
    expr { singleton_ast = $1; }
    ;

expr:
      NUMBER           { $$ = create_number($1); }
    | expr PLUS expr   { $$ = create_node_binary(AST_NODE_ADD, $1, $3); }
    | expr MINUS expr  { $$ = create_node_binary(AST_NODE_SUB, $1, $3); }
    | expr MUL expr  { $$ = create_node_binary(AST_NODE_MUL, $1, $3); }
    | expr DIV expr  { $$ = create_node_binary(AST_NODE_DIV, $1, $3); }
    ;

%%
