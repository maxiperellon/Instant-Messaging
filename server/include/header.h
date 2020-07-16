
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <pthread.h>
#include <mysql/mysql.h>

#define SERVER_PORT 8000
#define NUM_CLIENTS 10
#define MAXLINE 512

void* connection(void* d);
void cut_buff(char *subCad, char *cad, int inicio, int cuantos);
int search_client(char* user);
void* welcome(int id, char *buffer);
void* add_client(int id, char *buffer, char *name);
void* client_list(int id);
void* end_chat(int id, char *buffer);
void* chat_with_other_user(int id, char *buffer, char *name, char *temp);
void* chat_with_all_user(int id, char *buffer, char *buffer2, char *temp);

void insert_data(MYSQL *conn, char date, char username1, char username2);
void insert_conversation(MYSQL *conn, int conv, char date, char msg);

struct clients {
    int socket;
    char username[32];
    int sign_in;
    int status;
} s_cli[NUM_CLIENTS];
