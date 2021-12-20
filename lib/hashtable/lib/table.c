#include "../include/table.h"
#include <string.h>

struct Element {
    char * key;
    void * value;
};

void freeElement(struct node * n) {
    struct Element * elm = (struct Element *)n->data;
    free(elm->key);
    free(elm->value);
    free(elm);
    free(n);
}

unsigned long hash_function(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

struct HashTable * createTable(int size) {
    struct HashTable * table = (struct HashTable *)malloc(size*sizeof(struct HashTable));
    table->table = (struct LinkedList **)malloc(size * sizeof(struct LinkedList *));
    for (int i = 0; i < size; i++) {
        table->table[i] = createLinkedList(&freeElement);
    }
    table->size = size;
    pthread_mutex_t * mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(mutex, NULL);
    table->lock = mutex;
    return table;
}

void freeHashTable(struct HashTable * table) {

    for (int i = 0; i < table->size; i++) {
        freeLinkedList(table->table[i]);
    }
    free(table->table);
    pthread_mutex_destroy(table->lock);
    free(table->lock);
    free(table);
}

struct Element * getValueOf(struct LinkedList * list, char * key) {
    struct node * temp = list->n;
    while (temp != NULL) {
        struct Element * elm = (struct Element *)temp->data;
        if (strcmp(elm->key, key) == 0) {
            return elm;
        }
        temp = temp->next;
    }
    return NULL;
}


int removeKey(struct LinkedList * list, char * key) {
    struct node * temp = list->n;

    if (temp == NULL) {
        return 1;
    }

    struct Element * elm = (struct Element *)temp->data;
    if (strcmp(elm->key, key) == 0) {
        removeHead(list);
        return 0;
    }

    while (temp->next != NULL) {
        struct Element * elm = (struct Element *)temp->next->data;
        if (strcmp(elm->key, key) == 0) {
            temp->next = temp->next->next;
            return 0;
        }
        temp = temp->next;
    }
    return 1;
}

void * get(struct HashTable * table, char * key) {
    int index = hash_function(key)%table->size;
    pthread_mutex_lock(table->lock);
    struct Element * elm = (struct Element *)getValueOf(table->table[index], key);
    pthread_mutex_unlock(table->lock);
    if (elm != NULL) {
        return elm->value;
    }
    return NULL;
}

int insert(struct HashTable * table, char * key, char * value) {
    int index = hash_function(key)%table->size;
    pthread_mutex_lock(table->lock);
    struct Element * elm = getValueOf(table->table[index], key);
    if (elm == NULL) {
        elm = (struct Element *)malloc(sizeof(struct Element));
        elm->key = key;
        elm->value = value;
        insertHead(table->table[index], elm);
        pthread_mutex_unlock(table->lock);
        return 0;
    }
    pthread_mutex_unlock(table->lock);
    return 1;
}

int delete(struct HashTable * table, char * key) {
    int index = hash_function(key)%table->size;
    pthread_mutex_lock(table->lock);
    int res = removeKey(table->table[index], key);
    pthread_mutex_unlock(table->lock);
    return res;
}

void printTable(struct HashTable * table) {
    for (int i = 0; i < table->size; i++) {
        struct node * temp = table->table[i]->n;
        if (temp != NULL) {
            printf("Table Index %d: ", i);

            while (temp != NULL) {
                struct Element * elm = (struct Element *)temp->data;
                printf("{%s:%s} ", (char *)elm->key, (char *)elm->value);
                temp = temp->next;
            }
            printf("\n");
        }
    }
}