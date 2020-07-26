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
#include <time.h>

#define SERVER_PORT 8001
#define NUM_CLIENTS 10
#define MAX_LINE_LEN 512
#define MAX_NAME_LEN 32
#define MAX_QUERY_LEN 512

void connection(void* d);
void cut_buff(char *subCad, char *cad, int init, int c);
int search_client_by_name(char* name);
int welcome(int id, char *buffer);
int add_client(int id, char *buffer, char *name);
int client_list(int id);
int end_chat(int id, char *buffer);
int chat_with_other_user(int id, char *buffer, char *name, char *temp);

/* -----------------------------------DATABASE-------------------------------------- */

MYSQL *conn; /* variable de conexión para MySQL */
MYSQL_RES *res; /* variable que contendra el resultado de la consuta */
MYSQL_ROW row; /* variable que contendra los campos por cada registro consultado */

void insert_data(MYSQL *conn, char *date, char *username1, char *username2, char *msg);

/* -----------------------------------DATABASE-------------------------------------- */

/* -------------------------------------LOG----------------------------------------- */

int save_to_log(char *date, char *username1, char *username2, char *msg);

/* -------------------------------------LOG----------------------------------------- */

typedef struct _client {
    int id_client;
    int socket;
    char username[MAX_NAME_LEN];
    int sign_in;
    int status;
    int connected_to;
} client;

client s_cli[NUM_CLIENTS];