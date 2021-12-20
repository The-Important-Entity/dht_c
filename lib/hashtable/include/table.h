#ifndef TABLE_H
#define TABLE_H

#include "../lib/linkedlist/include/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct HashTable {
    struct LinkedList ** table;
    int size;
    pthread_mutex_t * lock;
};

unsigned long hash_function(char *str);
struct HashTable * createTable(int size);
void freeHashTable(struct HashTable * table);
void * get(struct HashTable * table, char * key);
int insert(struct HashTable * table, char * key, char * value);
int delete(struct HashTable * table, char * key);
char * allBindings(struct HashTable * table);
void printTable(struct HashTable * table);

#endif