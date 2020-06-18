#include "header.h"

MYSQL * ConnectionBD(void)
{
    MYSQL *conn; /* variable de conexión para MySQL */
    MYSQL_RES *res; /* variable que contendra el resultado de la consuta */
    MYSQL_ROW row; /* variable que contendra los campos por cada registro consultado */


    char *server = "localhost"; /*direccion del servidor 127.0.0.1re, localhost o direccion ip */
    char *user = "root"; /*usuario para consultar la base de datos */
    char *password = ""; /* contraseña para el usuario en cuestion */
    char *db = "instant-messaging"; /*nombre de la base de datos a consultar */

    /* enviar consulta SQL */
    conn = mysql_init(NULL); /*inicializacion a nula la conexión */

    if(!(conn = mysql_init(0))) { /*si la inicializacion de la conexion es nula */
        printf("no se puede conectar\n");
        exit(0);
    }

    /* conectar a la base de datos */
    if(!mysql_real_connect(conn, server, user, password, db, 0 , NULL, 0)) {
        printf("Imposible conectar con servidor mysql en el puerto\n " );
        mysql_close(conn);
        exit(1);
    }

    /* enviar la consulta SQL */
    if (mysql_query(conn, "show tables")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);
    /* imprimir los nombres de las tablas */
    printf("Tablas en la base de datos 'mysql':\n");
    while ((row = mysql_fetch_row(res)) != NULL)
        printf("%s \n", row[0]);
    /* liberar los recursos y cerrar la conexion */
    mysql_free_result(res);

    return conn;
}