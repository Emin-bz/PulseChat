#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENT_CONNS 2
#define MAX 2000
#define PORT 8888

typedef struct {
    int server_socket;
    int socket_addr_struct_size;
    int client_socket;
    struct sockaddr_in client_addr;
} client_connection_args;

typedef struct {
    u_int32_t id;
    u_int8_t alive;
    pthread_t* runner_space;
} chatroom;

int start_server();
int connect_client(client_connection_args* args);

#endif