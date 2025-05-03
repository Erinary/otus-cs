//
// Created by Liubov Udalova on 02.05.25.
//

#ifndef AST_H
#define AST_H

typedef enum {
    AST_NODE_NUMBER,
    AST_NODE_ADD,
    AST_NODE_SUB,
    AST_NODE_MUL,
    AST_NODE_DIV
} NodeType;

typedef struct AST {
    NodeType type;
    double value_number; // used only if type == AST_NODE_NUMBER
    int child_count;
    struct AST **children;  // Array of child pointers
} AST;

AST *singleton_ast;

// Constructors
AST *create_number(float value);

AST *create_node(NodeType type, int child_count, const AST *children[]);

AST *create_node_binary(NodeType type, const AST *left, const AST *right);

// Utility functions
void print_ast(AST *node, int indent);

void free_ast(AST *node);

double execute_ast(AST *node);

#endif //AST_H
