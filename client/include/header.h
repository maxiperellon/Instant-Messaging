#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_PORT 8000
#define MAXLINE 512

void *send_msg(void* s);
void *recv_msg(void* r);
