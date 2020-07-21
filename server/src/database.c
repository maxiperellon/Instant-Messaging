#include "header.h"
#include "config_database.h"

/* FUNCIONES PARA LA BASE DE DATOS */

/* Insertar datos de la conversacion a la base de BD */

void insert_data(MYSQL *conn, char date, char username1, char username2){
    char query[512];
    sprintf(consulta, "INSERT INTO `data` (`id_data`, `date`, `user1`, `user2`) VALUES (NULL, '%s', '%s', '%s');", date, name1, name2);
    if((mysql_query(conn, query)==0)){
    	printf("Se agregaron los datos de la conversacion a la BD \n");

    }else{
        printf("Error no se pudo agregar el mensaje. %s \n", mysql_error(conn));
    }
}

/* Insertar un mensaje a la BD en la conversación actual */

void insert_conversation(MYSQL *conn, int conv, char date, char msg){
    char query[512];
    sprintf(query, "INSERT INTO `conversation` (`id_conversation`, `data_id`, `date`, `msg`) VALUES (NULL, '%d', '%s', '%s');", conv, date, msg);
    if((mysql_query(conn, query)==0)){
    	printf("Se agrego un mensaje a la conversacion: %d \n" , conv);

    }else{
        printf("Error no se pudo agregar el mensaje. %s \n", mysql_error(conn));
    }
}


/* Obtener el ID de la última conversación */
int search_id_data_conversation(MYSQL *conn, MYSQL_ROW row, MYSQL_RES *res, char *query_db){
    //concatena las constantes a la variable consulta
    sprintf(query_db, "SELECT  * FROM `data` ORDER BY `id_data` DESC LIMIT 1");

    //hacemos consulta y verificamos que se cumple
    if((mysql_query(conn, query_db)==0)){

        //guardamos resultado en la variable resultado que es de tipo MYSQL_RES *
        res=mysql_store_result(conn);

        //leemos los datos almacenados en resultado y lo devolvemos a la variable row que es de tipo MYSQL_ROW
        while(row=mysql_fetch_row(res)){
            return atoi(row[0]);
        }
    }
    mysql_free_result(res);
    //preguntamos si se ha llegado al final de nuestra variable resultado
    if(!mysql_eof(res)){
        printf("Error de lectura %s\n", mysql_error(conn));
    }
}



/* Imprime en pantalla la conversación especificada *//*
void buscar_conversacion(MYSQL *conn, MYSQL_ROW row, MYSQL_RES *res,char *query, int id_data, int c){   
  char s[2024];
  char text[1024];
  sprintf(texto,"<<CONV -- ID:|HORA:  |MENSAJE: \n");
  chat_with_other_user(text,c);
  //concatena las constantes a la variable consulta 
  sprintf(consulta, "SELECT * FROM `conversation` WHERE `data_id` = '%d'", id);
 
  //hacemos consulta y verificamos que se cumple
  if((mysql_query(conn, consulta)==0)){
    //guardamos resultado en la variable resultado que es de tipo MYSQL_RES *
    res=mysql_store_result(conn);

    //leemos los datos almacenados en resultadoy lo devolvemos a la variable row que es de tipo MYSQL_ROW
    while(row=mysql_fetch_row(res)){                                           
        sprintf(s, "<<CONV -- %s | %s | %s \n",row[1], row[2], row[3]);
        chat_with_other_user(s, c);
    }
  }
  mysql_free_result(res);
  //preguntamos si se ha llegado al final de nuestra variable resultado
  if(!mysql_eof(res)){
    printf("Error de lectura %s\n", mysql_error(conn));
  }
}

*//* Muestra la lista de conversaciones creadas en la BD *//*
void lista_conversacion(MYSQL *conn, MYSQL_ROW row, MYSQL_RES *res, char *query, int c){   
  char s[2024];
  char texto[1024];
  sprintf(texto,"<<CONVER -- ID:|FECHA:      |USER1: |USER2: \n");
  chat_with_other_user(tex*//*t,c);

  //concatena las constantes a la variable consulta 
  sprintf(query, "SELECT * FROM `conversaciones`");

  //hacemos consulta y verificamos que se cumple
  if((mysql_query(conn, query)==0)){
    //guardamos resultado en la variable resultado que es de tipo MYSQL_RES *
    res=mysql_store_result(conn);
    //leemos los datos almacenados en resultadoy lo devolvemos a la variable row que es de tipo MYSQL_ROW
    while(row=mysql_fetch_row(res)){     
        sprintf(s, "<<CONVER -- %s | %s | %s | %s \n",row[0], row[1], row[2], row[3]);
        chat_with_other_user(s, c);    
    }
  }
  mysql_free_result(res);
  //preguntamos si se ha llegado al final de nuestra variable resultado
  if(!mysql_eof(res)){
    printf("Error de lectura %s\n", mysql_error(conn));
  }
}*/


