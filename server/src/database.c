#include "header.h"

/* FUNCIONES PARA LA BASE DE DATOS */

/* Insertar datos de la conversacion a la base de BD */

void insert_data(MYSQL *conn, char *date, char *username1, char *username2, char *msg){
    char query[512];
    sprintf(query, "INSERT INTO `data` (`id_data`, `date`, `user1`, `user2`, `msg`) VALUES (NULL, '%s', '%s', '%s', '%s');", date, username1, username2, msg);
    if((mysql_query(conn, query)==0)){
    	printf("Se agregaron los datos de la conversacion a la BD \n");

    }else{
        printf("Error no se pudo agregar el mensaje. %s \n", mysql_error(conn));
    }
}
