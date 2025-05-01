#ifndef LIST_H
#define LIST_H

struct NODE{
    void *data;
    struct NODE *next;
};
typedef struct NODE Node;

typedef struct {
    Node *head;
} List;

Node *create_node(void *value);
Node *add_first(Node *list, Node *newd);
Node *add_last(Node* list, Node *newd);
void add_after(Node *node, Node *newd);
void process(Node *list, void(*fn)(void *));
Node *find(Node *list, void *data);
Node * delete_node(Node *list, Node *element);
void display_list(const Node *head);
void delete_list(Node *head);


#endif // LIST_H
