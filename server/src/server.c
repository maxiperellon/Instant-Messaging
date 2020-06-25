#include "header.h"
#include "extern.h"

int main(int argc, char *argv[]) {

    struct sockaddr_in server_addr;
    struct sockaddr_in cli_addr;

    //CREACION DEL SOCKET
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    //AF_INET es el dominio y SOCK_STREAM se especifica que la conexion es TCP - socket de flujo
    if (socket_fd < 0){
        printf("\n--> Error al crear el socket...\n");
        return 1;
    }

    server_addr.sin_family = AF_INET; //tipo de conexión (por red o interna)
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //es la direccion del cliente al que se quiere atender -> INADDR_ANY (valor con el que se atiende a cualquier cliente)
    server_addr.sin_port = htons(SERVER_PORT); //es el numero que le corresponde al puerto

    /* Asociamos el socket a un puerto local */ //bind()
	if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) { /* Comprobamos si se asocio el socket a un puerto local */
    	printf("ERROR: error al asociar el socket al puerto local (%d)", ntohs(server_addr.sin_port));
    	close(socket_fd);
    	return 1;
  	}

    /* Ponemos el puerto a la escucha */ //listen()
	if (listen(socket_fd, NUM_CLIENTS) < 0) { /* Comprobamos si se puso a la escucha el puerto local */
		printf("ERROR: error al escuchar el puerto local (%d)", ntohs(server_addr.sin_port));
		return 1;
	}
	
	system("clear");
	printf("\n");
	printf("\tServidor CONECTADO...\n");
    printf("\tPUERTO: %d\n\n", ntohs(server_addr.sin_port));

    puts("\t\nEsperando las conexiones...");

    pthread_t thread;

    /*Aceptación de clientes*/
    while(1) {
        socklen_t cli_size = sizeof(cli_addr); /* Obtenemos el tamaño de la estructura en bytes (16)*/
		int conn_accept = accept(socket_fd, (struct sockaddr*)&cli_addr, &cli_size);
		//printf("prueba: %d", conn_accept);
		if (conn_accept < 0) {
		    perror("ERROR: error al conectarse al servidor..");
		    return 1;
		} else {
		    int id = clients;
		    s_cli[id].socket = conn_accept;
		    s_cli[id].sign_in = 0;
		    s_cli[id].status = 0;
		    clients++;
            if (pthread_create(&thread, NULL, connection, (void *)&id) < 0) {
                perror("ERROR: error al crear el hilo...");
                return 1;
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
	    sleep(1);
    }
}
