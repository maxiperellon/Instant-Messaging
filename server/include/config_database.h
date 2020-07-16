
#ifndef _CONFIG_DATABASE_H
#define _CONFIG_DATABASE_H

MYSQL *conn; /* variable de conexión para MySQL */
MYSQL_RES *res; /* variable que contendra el resultado de la consuta */
MYSQL_ROW row; /* variable que contendra los campos por cada registro consultado */

char *SERVER = "localhost"; /*direccion del servidor 127.0.0.1, localhost o direccion ip */
char *USER = "root"; /*usuario para consultar la base de datos */
char *PASSWORD = "root"; /* contraseña para el usuario en cuestion */
char *DATABASE = "chat"; /*nombre de la base de datos a consultar */

#endif //_CONFIG_DATABASE_H
