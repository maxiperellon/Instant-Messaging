//
// Created by Maximiliano Perellón on 28/01/2020.
//
#include "header.h"

void subCadena(char *subCad, char *cad, int inicio, int cuantos) {
    int i,j=0;
    for(i=inicio;i<inicio+cuantos && cad[i]!='\0';i++) {
        subCad[j]=cad[i];
        j++;
    }
    subCad[j]='\0';
}

int buscarCliente(char* user) {
    for(int i = 0; i < clients; i++) {
        if(strcmp(s_cli[i].user, user) == 0 && s_cli[i].sign_in == 1)
            return s_cli[i].socket;
    }
    return -1;
}

void* connection(void* d) {
    int *ide, id;
    ide = (int* ) d;
    id = *ide;
    char buffer[MAXLINE], name[MAXLINE-4], temp[MAXLINE-8];
    int i, longitud, destino;

    while(1) {
        recv(s_cli[id].socket, buffer, MAXLINE,0);
        printf("\nid%d\n", id);

        if(strstr(buffer, "ADD") && s_cli[id].sign_in == 0) {
            longitud = strlen(buffer);
            //Le quitamos el ADD
            subCadena(name, buffer, 4, longitud-4);
            strcpy(s_cli[id].user, name);
            //Se informa a todos menos a él mismo y al que se haya ido
            strcpy(buffer, "El usuario ");
            strcat(buffer, name);
            strcat(buffer, " ha entrado en el chat.");
            for(i = 0; i < clients; i++)
                if (i != id && s_cli[i].sign_in == 1)
                    send(s_cli[i].socket, buffer, MAXLINE, 0);
            s_cli[id].sign_in = 1;
        }
        if(strstr(buffer, "LIST") && s_cli[id].sign_in == 1) {
            //Se envia al cliente todos los usuarios menos los que hayan abandonado la sesión
            for(i = 0; i < clients; i++){
                if(s_cli[i].sign_in == 1)
                    send(s_cli[id].socket, s_cli[i].user, MAXLINE, 0);
            }
        }
        if(strstr(buffer, "END") && s_cli[id].sign_in == 1) {
            //Se informa a todos menos a él mismo y al que se haya ido
            strcpy(buffer, "El usuario ");
            strcat(buffer, s_cli[id].user);
            strcat(buffer, " ha abandonado en el chat.");
            bzero(s_cli[id].user, MAXLINE);
            for(i = 0; i < clients; i++)
                if (i != id && s_cli[i].sign_in == 1)
                    send(s_cli[i].socket, buffer, MAXLINE, 0);
            s_cli[id].sign_in = 0;
            exit(0);
        }
        if(strstr(buffer, "TEXT") && !strstr(buffer, "TEXT TO") && s_cli[id].sign_in == 1) {
            longitud = strlen(buffer);
            subCadena(temp, buffer, 5, longitud-5);
            //Se envía a todos menos a él mismo y al que se haya ido
            bzero(buffer, MAXLINE);
            strcat(buffer, s_cli[id].user);
            strcat(buffer, " dice: ");
            strcat(buffer, temp);
            for(i = 0; i < clients; i++)
                if (i != id && s_cli[i].sign_in == 1)
                    send(s_cli[i].socket, buffer, MAXLINE, 0);
        }
        if(strstr(buffer, "TEXT TO") && s_cli[id].sign_in == 1) {
            //Le quitamos el TEXT TO
            subCadena(name, buffer, 8, MAXLINE-8);
            //Nos quedamos sólo con el nombre, quitando desde el primer espacio en blanco hasta el final
            strtok(name, " ");
            //Se obtiene el socket destino
            destino = buscarCliente(name);
            longitud=strlen(name);
            strcpy(name, s_cli[id].user);
            strcat(name, " dice: ");
            //Recortamos el TEXT TO, el nombre, y los dos espacios hasta el mensaje(se suma solo uno (un espacio)
            //porque empieza a recorrer longitud(instruccion)+longitud(nombre) desde la pos. 0 de la cadena.
            subCadena(temp, buffer, 8+longitud+1, MAXLINE-(8+longitud+1));
            strcat(name, temp);
            if(destino != -1)
                send(destino, name, MAXLINE, 0);
        }
        fflush(stdout);
    }
    close(s_cli[id].socket);
    return 0;
}
