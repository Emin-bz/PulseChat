#ifndef SERVER_H
#define SERVER_H

#include<stdio.h>
#include<string.h> 
#include<sys/socket.h>
#include<arpa/inet.h> 

#define MAX 2000
#define PORT 8888

int start_server();

#endif