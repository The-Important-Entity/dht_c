#include "../include/underlay.h"
#include "../include/workers.h"
#include "../include/utils.h"
#include "hashtable/include/table.h"

#define WORKERS 100
#define FMAX 16
#define KEYMAX 32
#define VALUEMAX 32
#define INPUTMAX 128

static volatile int keepRunning = 1;
struct Server * server;

void sigintHandler(int signal) {
    if (fcntl(server->server_socket, F_GETFD) >= 0) {
        close(server->server_socket);
    }
    keepRunning = 0;
}

struct JobArguments {
    char * key;
    char * value;
    int client;
    struct HashTable * table;
    sem_t * jobs_sem;
};

void * getbinding(void * a){
    struct JobArguments * args = (struct JobArguments *)a;

    char * str = (char *)get(args->table, args->key);
    if (str == NULL) {
        char err[VALUEMAX];
        sprintf(err, "null");
        if (send(args->client, err, strlen(err), 0) < 0) {
            fprintf(stderr, "Error: Failed to send message to client %s\n", strerror(errno));
        }
    }
    else {
        if (send(args->client, str, strlen(str), 0) < 0) {
            fprintf(stderr, "Error: Failed to send message to client %s\n", strerror(errno));
        }
    }
    sem_post(args->jobs_sem);
    free(args->key);
    free(args->value);
    free(args);
    pthread_exit(NULL);
}

void * insertbinding(void * a){
    struct JobArguments * args = (struct JobArguments *)a;

    int res = insert(args->table, args->key, args->value);
    char response[VALUEMAX];
    if (res) {
        free(args->key);
        free(args->value);
        sprintf(response, "Error: Insert");
    }
    else {
        sprintf(response, "Success");
    }
    if (send(args->client, response, VALUEMAX, 0) < 0) {
        fprintf(stderr, "Error: Failed to send message to client %s\n", strerror(errno));
    }
    sem_post(args->jobs_sem);
    free(args);
    pthread_exit(NULL);
}

void * deletebinding(void * a){
    struct JobArguments * args = (struct JobArguments *)a;

    int res = delete(args->table, args->key);

    char response[VALUEMAX];
    if (res) {
        sprintf(response, "Error: Delete");
    } else {
        sprintf(response, "Success");
    }
    if (send(args->client, response, VALUEMAX, 0) < 0) {
        fprintf(stderr, "Error: Failed to send message to client %s\n", strerror(errno));
    }
    sem_post(args->jobs_sem);
    free(args->key);
    free(args->value);
    free(args);
    pthread_exit(NULL);
}

void * errorInput(void * a) {
    struct JobArguments * args = (struct JobArguments *)a;
    char err[INPUTMAX];
    sprintf(err, "Error: Input");
    if (send(args->client, err, INPUTMAX, 0) < 0) {
        fprintf(stderr, "Error: Failed to send message to client %s\n", strerror(errno));
    }
    sem_post(args->jobs_sem);
    free(args->key);
    free(args->value);
    free(args);
    pthread_exit(NULL);
}

void initThread(fun_ptr f, char * key, char * value, int client, struct HashTable * table, sem_t * jobs_sem){
    struct JobArguments * args = (struct JobArguments *)malloc(sizeof(struct JobArguments));
    args->key = key;
    args->value = value;
    args->client = client;
    args->table = table;
    args->jobs_sem = jobs_sem;

    pthread_t tid;
    pthread_create(&tid, NULL, f, (void *)args);
}

/*
get: foo
insert: foo bar
delete: foo
*/
void jobFactory(char * args, int client, struct HashTable * table, sem_t * jobs_sem){
    char * f = (char *)malloc(FMAX * sizeof(char)+1);
    char * key = (char *)malloc(KEYMAX * sizeof(char)+1);
    char * value = (char *)malloc(VALUEMAX * sizeof(char)+1);

    int res = sscanf(args, "%s %s %s", f, key, value);
    fun_ptr retval = &errorInput;
    if (res < 0 || strlen(f) == 0 || strlen(key) == 0) {
        goto EXIT;
    }
    if (strcmp(f, "get") == 0) {
        retval = &getbinding;
        goto EXIT;
    }
    if (strcmp(f, "insert") == 0) {
        if (strlen(value) == 0) {
            goto EXIT;
        }
        retval = &insertbinding;
        goto EXIT;
    }
    if (strcmp(f, "delete") == 0){
        retval = &deletebinding;
        goto EXIT;
    }
    goto EXIT;
EXIT:
    free(f);
    initThread(retval, key, value, client, table, jobs_sem);
}

void underlayLoop(int PORT){
    server = initServer(PORT);
    if (server == NULL) {
        freeServer(server);
        return;
    }

    struct HashTable * table = createTable(1000);
    sem_t * jobs_sem = initJobSem(WORKERS);

    int client_socket = -1;
    char buff[INPUTMAX];

    signal(SIGINT, sigintHandler);
    while (keepRunning) {
        if ((client_socket = acceptClient(server, buff, INPUTMAX)) < 0) {
            goto EXIT_ALL;
        }
        
        sem_wait(jobs_sem);
        jobFactory(buff, client_socket, table, jobs_sem);
        memset(buff, 0, INPUTMAX);
    }
EXIT_ALL:
    freeHashTable(table);
    freeSem(jobs_sem);
    freeServer(server);
    if (fcntl(client_socket, F_GETFD) >= 0) {
        close(client_socket);
    }
    return;
}