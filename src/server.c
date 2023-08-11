#include "server.h"

static uint32_t chatroom_id_counter = 0;

int start_server() {
    int server_socket , c;
    struct sockaddr_in server_addr , client_addr;
    
    server_socket = socket(AF_INET, SOCK_STREAM , 0);
    if (server_socket == -1)
    {
        printf("Could not create socket");
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
    
    listen(server_socket , 3);
    
    puts("Waiting for incoming connections...");

    uint32_t number_of_threads = 2;
    pthread_t *tid = calloc(number_of_threads, sizeof(pthread_t));
    
    client_connection_args args;
    args.server_socket = server_socket;
    args.socket_addr_struct_size = c;

    for(int i = 0; i < number_of_threads; i++) {
        pthread_create(&tid[i], NULL, (void*) &connect_client, &args);
    }

    for(int i = 0; i < number_of_threads; i++) {
        pthread_join(tid[i], NULL);
    }

    close(server_socket);

    return 0;
}


int connect_client(client_connection_args* args) {
    int client_socket;
    struct sockaddr_in client_address;
    char client_message[MAX];
    uint32_t chatroom_id;
    
    while( (client_socket = accept(args->server_socket, (struct sockaddr *)&client_address, (socklen_t*)&args->socket_addr_struct_size)) )
    {
        chatroom_id_counter++;
        chatroom_id = chatroom_id_counter;

        printf("Connection from client with socket address %d and descriptor %d accepted. Entering echo chat room %d...\n", client_address.sin_addr.s_addr, client_socket, chatroom_id);
        
        int stat = 0;
        while(1) {
            puts("Currently in message loop");
            memset(client_message, '\0', sizeof(client_message));

            // recv is blocking the thread until a message is receiced, so while(1) loop is safe.
            if(recv(client_socket, client_message, MAX, 0) <= 0)
            {
                printf("Recv from socket %d in chat room %d failed.\n", client_socket, chatroom_id);
                return 1;
            }
            
            if (strcmp(client_message, "exit\n") == 0) {
                printf("Client %d exited chat room %d.\n", client_socket, chatroom_id);
                close(args->server_socket);
                break;
            }
            
            printf("Message from client %d in chat room %d received: '%s'. Now sending it back...\n", client_socket, chatroom_id, client_message);
            
            if(send(client_socket, client_message, strlen(client_message), 0) < 0) {
                printf("Send from server %d in chat room %d to client socket %d failed.\n", args->server_socket, chatroom_id, client_socket);
                close(args->server_socket);
                return 1;
            }
        }
    }

    if (client_socket < 0)
    {
        perror("Could not accept any client connection");
        return 1;
    }
}