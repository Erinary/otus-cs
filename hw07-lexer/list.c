#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "scanner.h"

Node *create_node(void *value) {
    Node *result = (Node *) malloc(sizeof(Node));
    result->data = value;
    return result;
}

Node *add_first(Node *list, Node *newd) {
    newd->next = list;
    return newd;
}

Node *add_last(Node *list, Node *newd) {
    if (!list) {
        return newd;
    }
    Node *result = list;
    while (result->next) {
        result = result->next;
    }
    result->next = newd;
    return list;
}

void add_after(Node *node, Node *newd) {
    newd->next = node->next;
    node->next = newd;
}

void process(Node *list, void (*fn)(void *)) {
    Node *node = list;
    while (node) {
        fn(node->data);
        node = node->next;
    }
}

Node *find(Node *list, void *data) {
    Node *node = list;
    while (node) {
        const char *lex = (*(Token *) node->data).start;
        const char *find_lex = (*(Token *) data).start;
        if (strcmp(lex, find_lex) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

Node *delete_node(Node *list, Node *element) {
    Node *result = NULL;
    if (list == element) {
        result = element->next;
    } else {
        result = list;
        Node *node = list;
        while (node) {
            if (node->next == element) {
                node->next = element->next;
                break;
            }
            node = node->next;
        }
    }
    free(element);
    return result;
}

void display_list(const Node *head) {
    const Node *p = head;
    while (p != NULL) {
        const Token token = *(Token *) p->data;
        printf("%2d %2d '%.*s'\n", token.line, token.type, token.length, token.start); // [format]
        p = p->next;
    }
    printf("\n");
}

void delete_list(Node *head) {
    if (head == NULL)
        return;
    Node *p = head;
    while (p != NULL) {
        head = p;
        p = head->next;
        free(head);
    }
}
