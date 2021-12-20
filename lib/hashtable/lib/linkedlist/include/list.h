#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

struct node {
    void * data;
    struct node * next;
};

struct LinkedList {
    struct node * n;
    int size;
    void (*freeNodeCallback)(struct node *);
};

struct LinkedList * createLinkedList(void (*freeNode)(struct node *));
void freeLinkedList(struct LinkedList * list);
int isEmpty(struct LinkedList * list);
void * getHead(struct LinkedList * list);
void * getAt(struct LinkedList * list, int index);
void insertHead(struct LinkedList * list, void * data);
int insertAt(struct LinkedList * list, void * data, int index);
int removeHead(struct LinkedList * list);
int removeAt(struct LinkedList * list, int index);


#endif