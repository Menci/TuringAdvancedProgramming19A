#ifndef LIST_H_

#define LIST_H_

typedef struct listNode {
    struct listNode *pre;
    struct listNode *nxt;
    int id;
} listNode;

typedef struct {
    int size;
    struct listNode *node;
    struct listNode *cur;
} list_t;

list_t* newList(int size);
void moveForward(list_t *list, int step, int dir);
void deleteNode(list_t *list);
void deleteList(list_t *list);

#endif