#include "server.h"

int start_server() {
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;
    char client_message[MAX];
     
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
     
    if(bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    listen(socket_desc , 3);
     
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
         
        memset(client_message, '\0', sizeof(client_message));
        if(recv(client_sock, client_message, MAX, 0) < 0)
        {
            puts("Recv failed");
            return 1;
        }
         
        puts("Client message received. Now sending it back...");
         
        if(send(client_sock, client_message, strlen(client_message), 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
         
        puts("Message sent back to client");
    }
     
    if (client_sock < 0)
    {
        perror("Accept failed");
        return 1;
    }
     
    return 0;
}