#include <stdlib.h>

typedef struct linked_list_node {
    struct linked_list_node *prev, *next;
    int data;
}linked_list_node;

typedef struct {
    linked_list_node *head, *tail;
}linked_list;

linked_list *CreateList();
void DeleteList(linked_list *list);
int IsExisted(linked_list *list, linked_list_node *node);
void InsertNode(linked_list *list, linked_list_node *node, int data);
void DeleteNode(linked_list *list, linked_list_node *node);