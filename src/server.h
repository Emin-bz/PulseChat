#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h>
#include <pthread.h>

#define MAX 2000
#define PORT 8888
#define MAX_CLIENT_CONNS 10
#define MAX_CHATROOMS 3

typedef struct {
    int server_socket;
    int socket_addr_struct_size;
    int client_socket;
    struct sockaddr_in client_addr;
    int chatroom_id;
} client_connection_args;

typedef struct {
    u_int32_t id;
    u_int8_t alive;
    pthread_t* runner_space;
} chatroom;

int start_server();
int connect_client(void* args);

#endif