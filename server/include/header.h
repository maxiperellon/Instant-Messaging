
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <pthread.h>

#define SERVER_PORT 8000
#define NUM_CLIENTS 10
#define MAXLINE 512

void* connection(void* d);

struct clients {
    int socket;
    char user[MAXLINE-4];
    int sign_in;
} s_cli[NUM_CLIENTS];
