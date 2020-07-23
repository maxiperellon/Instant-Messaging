#include "header.h"
#include "extern.h"

void* connection(void* d) {
    int *ide, id;
    ide = (int* ) d;
    id = *ide;
    char buffer[MAXLINE], buffer2[MAXLINE], name[MAXLINE], temp[MAXLINE];

    while(1) {
        printf("\nid: %d\n", id);
        recv(s_cli[id].socket, buffer, MAXLINE,0);

        if(strstr(buffer, "ADD") && s_cli[id].sign_in == 0) {
            add_client(id, buffer, name);
            welcome(id, buffer);
        }

        if(strstr(buffer, "LIST") && s_cli[id].sign_in == 1 && s_cli[id].status == 0) {
            client_list(id);
        }

        if(strstr(buffer, "CHAT") && s_cli[id].sign_in == 1) {
            chat_with_other_user(id, buffer, name, temp);
        }

        if(strstr(buffer, "BYE") && s_cli[id].sign_in == 1) {
            end_chat(id, buffer);
        }

        fflush(stdout);
        sleep(3);
    }
    close(s_cli[id].socket);
    return 0;
}

void cut_buff(char *sub_cad, char *cad, int init, int c) {
    int i,j=0;
    for(i=init;i<init+c && cad[i]!='\0';i++) {
        sub_cad[j]=cad[i];
        j++;
    }
    sub_cad[j]='\0';
}

int search_socket_client(char* user) {
    for(int i = 0; i < clients; i++) {
        if(strcmp(s_cli[i].username, user) == 0 && s_cli[i].sign_in == 1){
            return s_cli[i].socket;
        }
    }
    return 0;
}

int search_id_client(char* user) {
    for(int i = 0; i < clients; i++) {
        if(strcmp(s_cli[i].username, user) == 0 && s_cli[i].sign_in == 1){
            return s_cli[i].id_client;
        }
    }
    return 0;
}

void* welcome(int id, char *buffer) {
    strcpy(buffer, "\tHola ");
    strcat(buffer, s_cli[id].username);
    strcat(buffer, "!!!");

    if (s_cli[id].sign_in == 1) {
        send(s_cli[id].socket, buffer, MAXLINE, 0);
    }
    return 0;
}

void* add_client(int id, char *buffer, char *name) {
    int len = strlen(buffer);
    //Le quitamos el ADD
    cut_buff(name, buffer, 4, len-4);
    strcpy(s_cli[id].username, name);
    //Se informa a todos menos a él mismo y al que se haya ido
    strcpy(buffer, "El usuario ");
    strcat(buffer, name);
    strcat(buffer, " ha ingresado en el chat.");
    for(int i = 0; i < clients; i++) {
        if (i != id && s_cli[i].sign_in == 1) {
            send(s_cli[i].socket, buffer, MAXLINE, 0);
        }
        s_cli[id].sign_in = 1;
    }
    return 0;
}

void* client_list(int id) {
    //Se envia al cliente todos los usuarios menos los que hayan abandonado la sesión y el de el propio
    for(int i = 0; i < clients; i++) {
        if(i != id && s_cli[i].sign_in == 1 && s_cli[i].status == 0) {
            send(s_cli[id].socket, s_cli[i].username, MAXLINE, 0);
        }
    }
    return 0;
}

void* end_chat(int id, char *buffer) {
    //Se informa a todos menos a él mismo y al que se haya ido
    strcpy(buffer, "El usuario ");
    strcat(buffer, s_cli[id].username);
    strcat(buffer, " ha abandonado el chat.");
    bzero(s_cli[id].username, MAXLINE);
    for (int i = 0; i < clients; i++) {
        if (i != id && s_cli[i].sign_in == 1) {
            send(s_cli[i].socket, buffer, MAXLINE, 0);
        }
        s_cli[id].sign_in = 0;
    }
    return 0;
}

void* chat_with_other_user(int id, char *buffer, char *name, char *temp) {

    /*------------- Fecha y hora ------------ */
    time_t t;
    struct tm *tm, *tm1;
    char date[100], hour[50];
    t=time(NULL);
    tm=localtime(&t);
    tm1=localtime(&t);
    strftime(date, 100, "%d/%m/%Y", tm);
    strftime(hour, 100, "%H:%M", tm1);
    /* -------------------------------------- */

    //Le quitamos la cadena CHAT
    cut_buff(name, buffer, 5, MAXLINE-5);
    //Nos quedamos sólo con el nombre, quitando desde el primer espacio en blanco hasta el final
    strtok(name, " ");
    //Se obtiene el socket destino
    int sock_destination = search_socket_client(name);
    int id_destination = search_id_client(name);

    char time[1000];
    strcpy(time, date);
    strcat(time, "  ");
    strcat(time, hour);

    int len=strlen(name);

    strcpy(name, time);
    strcat(name, " - ");
    strcat(name, s_cli[id].username);
    strcat(name, ": ");
    //Recortamos el CHAT, el nombre, y los dos espacios hasta el mensaje(se suma solo uno (un espacio)
    cut_buff(temp, buffer, 5+len+1, MAXLINE-(5+len+1));
    strcat(name, temp); // se envia el mensaje completo con el nick del usuario

    if (sock_destination != 0 && id_destination != -1){
        send(sock_destination, name, MAXLINE, 0);
        s_cli[id].status = 1;
        s_cli[id_destination].status = 1;

        //Se insertan los valores a la base de datos
        insert_data(conn, time, s_cli[id].username, s_cli[id_destination].username, temp);

        //Se guardan los datos en el log
        save_in_log(time, s_cli[id].username, s_cli[id_destination].username, temp);

    } else {
        char *msg = "El usuario con en el que desea chatear no se encuentra en la sala.";
        send(s_cli[id].socket, msg, MAXLINE, 0);
    }

    return 0;
}

char* save_in_log(char *date, char *username1, char *username2, char *msg){
    char string[128];

    FILE *fp;
    fp = fopen("chat.txt", "a+");

    strcpy(string, date);
    strcat(string," - user1: ");
    strcat(string, username1);
    strcat(string," - user2: ");
    strcat(string, username2);
    strcat(string," - mensaje: ");
    strcat(string, msg);
    strcat(string,"\n");
    fputs(string, fp);

    fclose(fp);

    return 0;
}