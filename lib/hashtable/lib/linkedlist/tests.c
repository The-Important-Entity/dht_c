#include "include/list.h"
#include <string.h>
#include <stdio.h>

void freeNode(struct node * node) {
    free(node->data);
    free(node);
}

void doNothing(struct node * node) {
    return;
}

struct Tester {
    int passed;
    int failed;
};

struct Tester * createTester() {
    struct Tester * t = (struct Tester *)malloc(sizeof(struct Tester));
    t->passed = 0;
    t->failed = 0;
    return t;
}

void Assert(struct Tester * T, const char * info, void * expected, void * received, int(ptr)(void *, void*)) {
    if (ptr(expected, received)) {
        printf("Test %d: Passed\n", T->passed++ + T->failed + 1);
    }
    else {
        printf("Test %d: Failed\n", T->passed + T->failed++ + 1);
    }
}

void printResults(struct Tester * T) {
    printf("\nTests Passed: %d\n", T->passed);
    printf("Tests Failed: %d\n", T->failed);
}

int compareStrings(void * s1, void * s2) {
    if (s1 == NULL || s2 == NULL) {
        if (s1 == s2) {
            return 1;
        }
        return 0;
    }
    return strcmp((char *)s1, (char *)s2) == 0; 
}

int compareInts(void * int1, void * int2) {
    return *(int *)int1 == *(int *)int2;
}

void runCharTests(struct Tester * Tester) {
    struct LinkedList * list = createLinkedList(&freeNode);
    
    char * data;
    char * received;


    received = getHead(list);
    Assert(Tester, "Test getHead when list is empty", NULL, received, &compareStrings);

    int res = isEmpty(list);
    int temp = 1;
    Assert(Tester, "Test isEmpty when list is empty", &temp, &res, &compareInts);

    data = (char *)malloc(3*sizeof(char)+1);
    strcpy(data, "001");
    insertHead(list, data);
    received = getHead(list);
    Assert(Tester, "Test getHead when list is not empty", data, received, &compareStrings);

    res = isEmpty(list);
    temp = 0;
    Assert(Tester, "Test isEmpty when list is not empty", &temp, &res, &compareInts);

    res = list->size;
    temp = 1;
    Assert(Tester, "Test list size when list is not empty", &temp, &res, &compareInts);


    data = (char *)malloc(3*sizeof(char)+1);
    strcpy(data, "002");
    insertHead(list, data);

    data = (char *)malloc(3*sizeof(char)+1);
    strcpy(data, "003");
    insertHead(list, data);

    data = (char *)malloc(3*sizeof(char)+1);
    strcpy(data, "004");
    insertHead(list, data);

    received = getAt(list, 0);
    const char * str1 = "004";
    Assert(Tester, "Test getAt when list is not empty", received, (char *)str1, &compareStrings);

    received = getAt(list, 1);
    const char * str2 = "003";
    Assert(Tester, "Test getAt when list is not empty", received, (char *)str2, &compareStrings);

    received = getAt(list, 2);
    const char * str3 = "002";
    Assert(Tester, "Test getAt when list is not empty", received, (char *)str3, &compareStrings);

    received = getAt(list, 3);
    const char * str4 = "001";
    Assert(Tester, "Test getAt when list is not empty", received, (char *)str4, &compareStrings);

    res = list->size;
    temp = 4;
    Assert(Tester, "Test list size when list is not empty", &temp, &res, &compareInts);

    
    freeLinkedList(list);
}
int main() {
    struct Tester * T = createTester();
    runCharTests(T);
    printResults(T);
    free(T);
    return 0;
}