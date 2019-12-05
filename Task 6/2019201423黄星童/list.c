#include <stdlib.h>
#include "list.h"

linked_list *CreateList() {
    linked_list *newList = malloc(sizeof(linked_list));
    newList -> head = newList -> tail = NULL;
    return newList;
}

void DeleteList(linked_list *list) {
    linked_list_node *node = list -> head;
    while (node != list -> tail) {
        linked_list_node *tmp = node;
        node = node -> next;
        free(tmp);
    }
    if (node != NULL) free(node);
    free(list);
    return;
}

int IsExisted(linked_list *list, linked_list_node *node) {
    if (list -> head == NULL)
        return 0;
    if (list -> tail == node)
        return 1;

    linked_list_node *now = list -> head;
    while (now != list -> tail) {
        if (now == node) return 1;
        now = now -> next;
    }

    return 0;
}

void InsertNode(linked_list *list, linked_list_node *node, int data) {                      // Insert newNode after node.
    linked_list_node *newNode = malloc(sizeof(linked_list_node));
    newNode -> data = data;

    if (list -> head == NULL) {
        list -> head = list -> tail = newNode;
        newNode -> prev = newNode -> next = newNode;
        return;
    }
    if (node == list -> tail)
        list -> tail = newNode;

    newNode -> next = node -> next;
    newNode -> next -> prev = newNode;
    node -> next = newNode;
    newNode -> prev = node;
    return;
}

void DeleteNode(linked_list *list, linked_list_node *node) {
    if (list -> head == list -> tail) {
        list -> head = list -> tail = NULL;
        free(node);
        return;
    }
    if (node == list -> head)
        list -> head = node -> next;
    if (node == list -> tail)
        list -> tail = node -> prev;

    node -> prev -> next = node -> next;
    node -> next -> prev = node -> prev;
    free(node);
    return;
}