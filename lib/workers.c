#include "../include/workers.h"
#include "../include/utils.h"

void freeServer(struct Server * server) {
    if (fcntl(server->server_socket, F_GETFD) >= 0) {
        close(server->server_socket);
    }
    free(server);
    server = NULL;
}

struct Server * initServer(int PORT){
    struct Server * server = (struct Server *)malloc(sizeof(struct Server));
    server->addrlen = sizeof(struct sockaddr_in);
    
    if ((server->server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Error: Failed to create socket. %s.\n", strerror(errno));
        return NULL;
    }
    int opt = 0;

    if (setsockopt(server->server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) != 0) {
        fprintf(stderr, "Error: Failed to set socket options. %s.\n", strerror(errno));
        return NULL;
    }
    memset(&server->server_addr, 0, server->addrlen);         // Zero out structure
    server->server_addr.sin_family = AF_INET;         // Internet address family
    server->server_addr.sin_addr.s_addr = INADDR_ANY; // Internet address, 32 bits
    server->server_addr.sin_port = htons(PORT);       // Server port, 16 bits
    // Bind to the local address.
    if (bind(server->server_socket, (struct sockaddr *)&server->server_addr, server->addrlen) < 0) {
        fprintf(stderr, "Error: Failed to bind socket to port %d. %s.\n", PORT, strerror(errno));
        return NULL;
    }
    // Mark the socket so it will listen for incoming connections.
    if (listen(server->server_socket, SOMAXCONN) < 0) {
        fprintf(stderr, "Error: Failed to listen for incoming connections. %s.\n", strerror(errno));
        return NULL;
    }
    printf("Echo server is up and running on port %d...\n", ntohs(server->server_addr.sin_port));
    return server;
}

int acceptClient(struct Server * server, char * buff, int INPUTMAX){
    int client_socket;
    int bytes_recvd;
    struct sockaddr_in client_addr;

    if ((client_socket = accept(server->server_socket, (struct sockaddr *)&client_addr, &server->addrlen)) < 0) {
        fprintf(stderr, "Error: Failed to accept incoming connection. %s.\n", strerror(errno));
        return -1;
    }
    if ((bytes_recvd = recv(client_socket, buff, INPUTMAX - 1, 0)) < 0) {
        fprintf(stderr, "Error: Failed to receive message from client %s. %s.\n", inet_ntoa(client_addr.sin_addr), strerror(errno));
        return -1;
    }
    buff[bytes_recvd] = '\0';
    printf("Received message from client [%s:%d]: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buff);
    return client_socket;
}