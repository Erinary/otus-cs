//
// Created by Liubov Udalova on 02.05.25.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "symbol_table.h"

AST *singleton_ast;

AST *create_node(const NodeType type, const int child_count, const AST *children[]) {
    AST *node = malloc(sizeof(AST));
    if (!node) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    node->type = type;
    node->value_number = 0;
    node->child_count = child_count;
    node->children = NULL;

    if (child_count > 0) {
        node->children = malloc(sizeof(AST *) * child_count);
        if (!node->children) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < child_count; ++i) {
            node->children[i] = (AST *) children[i];
        }
    }
    return node;
}

AST *create_node_binary(const NodeType type, const AST *left, const AST *right) {
    return create_node(type, 2, (const AST *[]){left, right});
}

AST *create_number(const float value) {
    AST *node = malloc(sizeof(AST));
    if (!node) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    node->type = AST_NODE_NUMBER;
    node->value_number = value;
    node->child_count = 0;
    node->children = NULL;

    return node;
}

AST *create_ident(char *name) {
    AST *node = create_node(AST_NODE_ID, 0, (const AST *[]){});
    node->value_str = strdup(name);
    return node;
};

AST *create_assignment(AST *name, AST *expr) {
    return create_node(AST_NODE_ASSIGNMENT, 2, (const AST *[]){name, expr});
}

void free_ast(AST *node) {
    // TODO FIX: Implement
}

double execute_ast(const AST *node) {
    switch (node->type) {
        case AST_NODE_NUMBER:
            return node->value_number;
        case AST_NODE_ID:
            SymbolLookupResult lookup = lookup_variable(node->value_str);
            if (!lookup.found) {
                fprintf(stderr, "Unknown id %s\n", node->value_str);
                exit(EXIT_FAILURE);
            }
            return lookup.value;
        case AST_NODE_ADD:
            return execute_ast(node->children[0]) + execute_ast(node->children[1]);
        case AST_NODE_SUB:
            return execute_ast(node->children[0]) - execute_ast(node->children[1]);
        case AST_NODE_MUL:
            return execute_ast(node->children[0]) * execute_ast(node->children[1]);
        case AST_NODE_DIV:
            return execute_ast(node->children[0]) / execute_ast(node->children[1]);
        case AST_NODE_ASSIGNMENT:
            double result = execute_ast(node->children[1]);
            assign_variable(node->children[0]->value_str, result);
            return result;
        default:
            fprintf(stderr, "Unknown node type %d\n", node->type);
            exit(EXIT_FAILURE);
    }
}

void print_ast(AST *node, int indent) {
    if (!node) {
        return;
    }
    for (int i = 0; i < indent; ++i) {
        fprintf(stderr, "  ");
    }
    switch (node->type) {
        case AST_NODE_NUMBER:
            fprintf(stderr, "Number(%g)\n", node->value_number);
            break;
        case AST_NODE_ADD:
            fprintf(stderr, "Add\n");
            break;
        case AST_NODE_SUB:
            fprintf(stderr, "Sub\n");
            break;
        case AST_NODE_MUL:
            fprintf(stderr, "Mul\n");
            break;
        case AST_NODE_DIV:
            fprintf(stderr, "Div\n");
            break;
        case AST_NODE_ASSIGNMENT:
            fprintf(stderr, "Assign\n");
            break;
        case AST_NODE_ID:
            fprintf(stderr, "Id(%s)\n", node->value_str);
            break;
        default:
            fprintf(stderr, "Unknown node type: %d\n", node->type);
    }
    for (int i = 0; i < node->child_count; ++i) {
        print_ast(node->children[i], indent + 1);
    }
}
