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

void* chat_with_other_user(int id, char *buffer, char *user, char *temp) {
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

/* FUNCIONES PARA LA BASE DE DATOS */


/* Insertar un mensaje a la BD en la conversación actual */
void insertar_msg(MYSQL *conn, int conv char *temp, char *buffer){
    char consulta[512];
    sprintf(consulta, "INSERT INTO `mensajes` (`id`, `conversacion_id`, `date`, `msg`) VALUES (NULL, '%d', '%s', '%s');", conv, temp, buffer);
    if((mysql_query(conn, consulta)==0)){
        printf("Se agrego un mensaje a la conversacion: %d \n", conv);

    }else{
        printf("Error no se pudo agregar el mensaje. %s \n", mysql_error(connection));
    }
}

void insert_client(MYSQL *conn, char *name, char *temp){
    char query[512];
    sprintf(query, "INSERT INTO `users` (`id`, `name`, `date`) VALUES (NULL, '%s', '%s');", name, temp);
    if((mysql_query(conn, query)==0)){
        printf("Nuevo usuario agregado: %s \n", name);
    }else{
        printf("Usuario: %s \n", mysql_error(conn));
    }

}
int found_name(MYSQL *conn, MYSQL_ROW row, MYSQL_RES *res, char *query, char *name){
    char id_nick;
    sprintf(query, "SELECT `id` FROM users where `name` like '%s' ORDER BY `id` desc limit 1;", name);
    //hacemos consulta y verificamos que se cumple
    if((mysql_query(conn, query) == 0)){
        //guardamos resultado en la variable resultado que es de tipo MYSQL_RES *
        res=mysql_store_result(conn);
        //leemos los datos almacenados en resultado y lo devolvemos a la variable row que es de tipo MYSQL_ROW
        while(row=mysql_fetch_row(res)){
            return atoi(row[1]);
        }
    }

    mysql_free_result(res);
    //preguntamos si se ha llegado al final de nuestra variable resultado
    if(!mysql_eof(res)){
        printf("Error de lectura %s\n", mysql_error(conn));
    }
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
