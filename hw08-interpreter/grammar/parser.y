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
    char *value_str;
    AST *node;
}

%token <value_number> NUMBER
%token <value_str> IDENT
%token PLUS MINUS MUL DIV PARENTHESIS_LEFT PARENTHESIS_RIGHT ASSIGN // + - * / ( ) =
%type <node> expr
%type <node> stmt

%start input

%right ASSIGN
%left PLUS MINUS
%left MUL DIV

%%

input:
    stmt { singleton_ast = $1; }
    ;

stmt:
      IDENT ASSIGN expr { $$ = create_assignment(create_ident($1), $3); }
    | expr { $$ = $1; }
    ;

expr:
      NUMBER           { $$ = create_number($1); }
    | IDENT            { $$ = create_ident($1); }
    | expr PLUS expr   { $$ = create_node_binary(AST_NODE_ADD, $1, $3); }
    | expr MINUS expr  { $$ = create_node_binary(AST_NODE_SUB, $1, $3); }
    | expr MUL expr  { $$ = create_node_binary(AST_NODE_MUL, $1, $3); }
    | expr DIV expr  { $$ = create_node_binary(AST_NODE_DIV, $1, $3); }
    | PARENTHESIS_LEFT expr PARENTHESIS_RIGHT { $$ = $2; }
    ;

%%
