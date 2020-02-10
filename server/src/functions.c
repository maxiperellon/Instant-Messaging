//
// Created by Maximiliano Perellón on 28/01/2020.
//
#include "header.h"
#include "extern.h"

void subCadena(char *subCad, char *cad, int inicio, int cuantos) {
    int i,j=0;
    for(i=inicio;i<inicio+cuantos && cad[i]!='\0';i++) {
        subCad[j]=cad[i];
        j++;
    }
    subCad[j]='\0';
}

int search_client(char* user) {
    for(int i = 0; i < clients; i++) {
        if(strcmp(s_cli[i].user, user) == 0 && s_cli[i].sign_in == 1)
            return s_cli[i].socket;
    }
    return -1;
}

void* add_client(int id, char *buffer, char *name) {
    int len = strlen(buffer);
    //Le quitamos el ADD
    subCadena(name, buffer, 4, len-4);
    strcpy(s_cli[id].user, name);
    //Se informa a todos menos a él mismo y al que se haya ido
    strcpy(buffer, "El usuario ");
    strcat(buffer, name);
    strcat(buffer, " ha entrado en el chat.");
    for(int i = 0; i < clients; i++) {
        if (i != id && s_cli[i].sign_in == 1) {
            send(s_cli[i].socket, buffer, MAXLINE, 0);
        }
        s_cli[id].sign_in = 1;
    }
    return 0;
}

void* client_list(int id) {
    //Se envia al cliente todos los usuarios menos los que hayan abandonado la sesión
    for(int i = 0; i < clients; i++) {
        if(s_cli[i].sign_in == 1) {
            send(s_cli[id].socket, s_cli[i].user, MAXLINE, 0);
        }
    }
    return 0;
}

void* end_chat(int id, char *buffer) {
    //Se informa a todos menos a él mismo y al que se haya ido
    strcpy(buffer, "El usuario ");
    strcat(buffer, s_cli[id].user);
    strcat(buffer, " ha abandonado en el chat.");
    bzero(s_cli[id].user, MAXLINE);
    for(int i = 0; i < clients; i++) {
        if (i != id && s_cli[i].sign_in == 1) {
            send(s_cli[i].socket, buffer, MAXLINE, 0);
        }
        s_cli[id].sign_in = 0;
    }
    return 0;
}

void* chat_with_other_user(int id, char *buffer, char *name, char *temp) {
    //Le quitamos el TEXT TO
    subCadena(name, buffer, 8, MAXLINE-8);
    //Nos quedamos sólo con el nombre, quitando desde el primer espacio en blanco hasta el final
    strtok(name, " ");
    //Se obtiene el socket destino
    int destino = search_client(name);
    int len=strlen(name);
    strcpy(name, s_cli[id].user);
    strcat(name, " dice: ");
    //Recortamos el TEXT TO, el nombre, y los dos espacios hasta el mensaje(se suma solo uno (un espacio)
    //porque empieza a recorrer longitud(instruccion)+longitud(nombre) desde la pos. 0 de la cadena.
    subCadena(temp, buffer, 8+len+1, MAXLINE-(8+len+1));
    strcat(name, temp);
    if(destino != -1)
        send(destino, name, MAXLINE, 0);

    return 0;
}

void* chat_with_all_user(int id, char *buffer, char *temp) {
    int len = strlen(buffer);
    subCadena(temp, buffer, 5, len-5);
    //Se envía a todos menos a él mismo y al que se haya ido
    bzero(buffer, MAXLINE);
    strcat(buffer, s_cli[id].user);
    strcat(buffer, " dice: ");
    strcat(buffer, temp);
    for(int i = 0; i < clients; i++)
        if (i != id && s_cli[i].sign_in == 1)
            send(s_cli[i].socket, buffer, MAXLINE, 0);
    return 0;
}

void* connection(void* d) {
    int *ide, id;
    ide = (int* ) d;
    id = *ide;
    char buffer[MAXLINE], name[MAXLINE-4], temp[MAXLINE-8];

    while(1) {
        printf("\nid: %d\n", id);
        recv(s_cli[id].socket, buffer, MAXLINE,0);

        if(strstr(buffer, "ADD") && s_cli[id].sign_in == 0) {
            add_client(id, buffer, name);
        }

        if(strstr(buffer, "LIST") && s_cli[id].sign_in == 1) {
            client_list(id);
        }

        if(strstr(buffer, "END") && s_cli[id].sign_in == 1) {
            end_chat(id, buffer);
        }

        if(strstr(buffer, "TEXT") && !strstr(buffer, "TEXT TO") && s_cli[id].sign_in == 1) {
            chat_with_all_user(id, buffer, temp);
        }

        if(strstr(buffer, "TEXT TO") && s_cli[id].sign_in == 1) {
            chat_with_other_user(id, buffer, name, temp);
        }
        fflush(stdout);
        sleep(3);
    }
    close(s_cli[id].socket);
    return 0;
}
