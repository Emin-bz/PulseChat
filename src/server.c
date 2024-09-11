#include "server.h"

static uint32_t chatroom_id_counter = 0;
static uint32_t last_exited_chatroom_id = -1;


int start_server() {
    int server_socket, client_socket, c;
    struct sockaddr_in server_addr, client_addr;
    
    server_socket = socket(AF_INET, SOCK_STREAM , 0);
    if (server_socket == -1)
    {
        printf("Could not create socket");
        return 1;
    }
    puts("Socket created");
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
     
    if (bind(server_socket,(struct sockaddr *)&server_addr , sizeof(server_addr)) < 0)
    {
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    listen(server_socket , MAX_CLIENT_CONNS);
    
    puts("Waiting for incoming connections...");

    pthread_t *tids = calloc(MAX_CLIENT_CONNS, sizeof(pthread_t));
    chatroom chatrooms[MAX_CLIENT_CONNS] = {0};
    uint32_t chatroom_counter = 0;

    char client_message[MAX];
    while( (client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t*)&c)) ) {
        if (chatroom_counter >= MAX_CLIENT_CONNS) {
            printf("Maximum number of client connections reached. Cannot create a new chat room for client %d", client_socket);
            close(client_socket);
            continue;
        }

        uint32_t room_id = chatroom_id_counter++;
        chatrooms[chatroom_counter] = (chatroom) {.id = room_id, .alive = 1, .runner_space = &tids[chatroom_counter]};
        
        client_connection_args *args = malloc(sizeof(client_connection_args));
        args->server_socket = server_socket;
        args->socket_addr_struct_size = c;
        args->client_socket = client_socket;
        args->client_addr = client_addr;
        args->chatroom_id = room_id;

        if (pthread_create(chatrooms[chatroom_counter].runner_space, NULL, (void*) &connect_client, args) != 0) {
            perror("pthread_create failed");
            free(args);
            close(client_socket);
            continue;
        }
        pthread_detach(*chatrooms[chatroom_counter].runner_space);
        chatroom_counter++;
    }
    
    close(server_socket);
    free(tids);

    return 0;
}


int connect_client(void* arg) {
    client_connection_args* args = (client_connection_args*)arg;
    int client_socket = args->client_socket;
    struct sockaddr_in client_address = args->client_addr;
    char client_message[MAX];
    uint32_t chatroom_id = args->chatroom_id;

    char client_addr_str[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &client_address.sin_addr.s_addr, client_addr_str, INET_ADDRSTRLEN) == NULL) {
        perror("inet_ntop");
        return 1;
    }

    printf("Connection from client with socket address %s:%d and descriptor %d accepted. Entering echo chat room %d...\n", client_addr_str, client_address.sin_port, client_socket, chatroom_id);
    
    while(1) {
        puts("Currently in message loop");
        memset(client_message, '\0', sizeof(client_message));

        // recv is blocking the thread until a message is received, so while(1) loop is safe.
        if(recv(client_socket, client_message, MAX, 0) <= 0) {
            printf("Recv from socket %d in chat room %d failed.\n", client_socket, chatroom_id);
            break;
        }
        
        if (strcmp(client_message, "exit\n") == 0) {
            printf("Client %d exited chat room %d.\n", client_socket, chatroom_id);
            break;
        }
        
        printf("Message from client %d in chat room %d received: '%s'. Now sending it back...\n", client_socket, chatroom_id, client_message);
        
        if(send(client_socket, client_message, strlen(client_message), 0) < 0) {
            printf("Send from server %d in chat room %d to client socket %d failed.\n", args->server_socket, chatroom_id, client_socket);
            close(args->server_socket);
            break;
        }
    }

    close(client_socket);
    free(args); // Free the dynamically allocated memory

    return 0;
}