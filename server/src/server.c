#include "header.h"
#include "extern.h"
#include "error.h"
#include "database.h"

int main(int argc, char *argv[]) {

    MYSQL *conn; /* variable de conexión para MySQL */
    MYSQL_RES *res; /* variable que contendra el resultado de la consuta */
    MYSQL_ROW row; /* variable que contendra los campos por cada registro consultado */
    conn = mysql_init(NULL); /*inicializacion a nula la conexión */

    /* conectar a la base de datos */
    if (!mysql_real_connect(conn, SERVER, USER, PASSWORD, DATABASE, 0, NULL, 0)) { /* definir los parámetros de la conexión antes establecidos */
        fprintf(stderr, "%s\n", mysql_error(conn)); /* si hay un error definir cual fue dicho error */
        exit(1);
    } else {
        printf("Base de datos conectada...")
    }

    /* enviar consulta SQL */
    if (mysql_query(conn, "select * from datos")) { /* definicion de la consulta y el origen de la conexion */
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);
    printf("ID\tNombre\t\tedad\n");
    while ((row = mysql_fetch_row(res)) != NULL) /* recorrer la variable res con todos los registros obtenidos para su uso */
        printf("%s\t%s\t%s \n", row[0],row[1],row[2]); /* la variable row se convierte en un arreglo por el numero de campos que hay en la tabla */

    /* se libera la variable res y se cierra la conexión */
    mysql_free_result(res);
    mysql_close(conn);

    struct sockaddr_in server_addr;
    struct sockaddr_in cli_addr;

    //CREACION DEL SOCKET
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    //AF_INET es el dominio y SOCK_STREAM se especifica que la conexion es TCP - socket de flujo
    if (socket_fd < 0){
        error_socket();
    }

    server_addr.sin_family = AF_INET; //tipo de conexión (por red o interna)
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //es la direccion del cliente al que se quiere atender -> INADDR_ANY (valor con el que se atiende a cualquier cliente)
    server_addr.sin_port = htons(SERVER_PORT); //es el numero que le corresponde al puerto
    int port = ntohs(server_addr.sin_port);

    /* Asociamos el socket a un puerto local */ //bind()
	if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) { /* Comprobamos si se asocio el socket a un puerto local */
        error_bind(port, socket_fd);
  	}

    /* Ponemos el puerto a la escucha */ //listen()
	if (listen(socket_fd, NUM_CLIENTS) < 0) { /* Comprobamos si se puso a la escucha el puerto local */
        error_listen(port);
	}
	
	system("clear");
	printf("\n");
	printf("\tServidor CONECTADO...\n");
    printf("\tPUERTO: %d\n\n", port);

    puts("\t\nEsperando las conexiones...");

    pthread_t thread;

    /*Aceptación de clientes*/
    while(1) {
        socklen_t cli_size = sizeof(cli_addr); /* Obtenemos el tamaño de la estructura en bytes (16)*/
		int conn_accept = accept(socket_fd, (struct sockaddr*)&cli_addr, &cli_size);
		//printf("prueba: %d", conn_accept);
		if (conn_accept < 0) {
            error_conn_accept();
		} else {
		    int id = clients;
		    s_cli[id].socket = conn_accept;
		    s_cli[id].sign_in = 0;
		    s_cli[id].status = 0;
		    clients++;
            if (pthread_create(&thread, NULL, connection, (void *)&id) < 0) {
                error_thread_create();
            } else {
                if (clients <= NUM_CLIENTS){
                    printf("Hilo del cliente creado correctamente, su id: %d", id);
                } else {
                    printf("Se supero la cantidad máxima de clientes..");
                    close(conn_accept);
                    continue;
                }

            }
        }
	    //sleep(1);
    }

    close(socket_fd);
    printf("SERVIDOR FINALIZADO..\n");

    return 0;
}
