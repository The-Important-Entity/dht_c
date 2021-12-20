#include "../include/list.h"

struct node * createNode(){
    return (struct node *)malloc(sizeof(struct node));
}

void freeAllNodes(struct node * node, void (*freeNode)(struct node *)) {
    if (node != NULL) {
        freeAllNodes(node->next, freeNode);
        freeNode(node);
    }
}

struct LinkedList * createLinkedList(void (*freeNode)(struct node *)) {
    struct LinkedList * list = (struct LinkedList *)malloc(sizeof(struct LinkedList));
    list->n = NULL;
    list->size = 0;
    list->freeNodeCallback = freeNode;
    return list;
}

void freeLinkedList(struct LinkedList * list) {
    freeAllNodes(list->n, list->freeNodeCallback);
    free(list);
}

int isEmpty(struct LinkedList * list) {
    return list->size == 0;
}

void * getHead(struct LinkedList * list) {
    if (list->n == NULL) {
        return NULL;
    }
    return list->n->data;
}

void * getAt(struct LinkedList * list, int index) {
    struct node * temp = list->n;

    while(temp != NULL && index > 0) {
        temp = temp->next;
        index--;
    }
    if (index == 0 && temp != NULL) {
        return temp->data;
    }
    return NULL;
}

void insertHead(struct LinkedList * list, void * data) {
    struct node * n = createNode();
    n->data = data;
    n->next = list->n;
    list->n = n;
    list->size++;
}

int insertAt(struct LinkedList * list, void * data, int index) {
    if (index == 0) {
        insertHead(list, data);
        return 0;
    }
    if (list->n == NULL) {
        return 1;
    }
    struct node * temp = list->n;
    while (temp->next != NULL && index > 1) {
        temp = temp->next;
        index--;
    }
    if (index != 1) {
        return 1;
    }
    struct node * node = createNode();
    node->data = data;
    node->next = temp->next;
    temp->next = node;
    list->size++;
    return 0;
}

int removeHead(struct LinkedList * list) {
    if (list->n == NULL) {
        return 1;
    }

    struct node * temp = list-> n;
    list->n = list->n->next;
    list->freeNodeCallback(temp);
    list->size--;
    return 0;
}

int removeAt(struct LinkedList * list, int index) {
    if (index == 0) {
        return removeHead(list);
    }
    if (list->n == NULL) {
        return 1;
    }

    struct node * current = list->n;
    while(current->next != NULL && index > 1) {
        current = current->next;
        index--;
    }
    if (index!=1) {
        return 1;
    }
    struct node * temp = current->next;
    current->next = current->next->next;
    list->freeNodeCallback(temp);
    list->size--;
    return 0;
}