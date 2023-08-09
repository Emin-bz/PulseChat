#include "server.h"

int start_server() {
    int server_socket , client_socket , c;
    struct sockaddr_in server_addr , client_addr;
    char client_message[MAX];
    
    server_socket = socket(AF_INET , SOCK_STREAM , 0);
    if (server_socket == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
     
    if(bind(server_socket,(struct sockaddr *)&server_addr , sizeof(server_addr)) < 0)
    {
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    listen(server_socket , 3);
    
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    
    while( (client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
        printf("Socket val: %d\n", client_socket);
         
        memset(client_message, '\0', sizeof(client_message));
        if(recv(client_socket, client_message, MAX, 0) < 0)
        {
            puts("Recv failed");
            return 1;
        }
        
        puts("Client message received. Now sending it back...");
         
        if(send(client_socket, client_message, strlen(client_message), 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
         
        puts("Message sent back to client");
    }
     
    if (client_socket < 0)
    {
        perror("Accept failed");
        return 1;
    }
     
    return 0;
}