#ifndef SEM_LIB
#define SEM_LIB

#include <semaphore.h>

#endif

#ifndef WORKERS_H
#define WORKERS_H

#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include "sem.h"

struct Server {
    int server_socket;
    struct sockaddr_in server_addr;
    socklen_t addrlen;
};

void freeServer(struct Server * server);
struct Server * initServer(int PORT);

int acceptClient(struct Server * server, char * buff, int INPUTMAX);

#endif