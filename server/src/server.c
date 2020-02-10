#include "header.h"
#include "extern.h"

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr;
    struct sockaddr_in cli_addr;

    //CREACION DEL SOCKET
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0){
        printf("\n--> Error al crear el socket...\n");
        exit(0);
    }

    server_addr.sin_family = AF_INET; //tipo de conexión (por red o interna)
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //es la direccion del cliente al que se quiere atender -> INADDR_ANY (valor con el que se atiende a cualquier cliente)
    server_addr.sin_port = htons(SERVER_PORT); //es el numero que le corresponde al puerto

    /* Asociamos el socket a un puerto local */
	int b = bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (b < 0) { /* Comprobamos si se asocio el socket a un puerto local */
    	printf("ERROR: error al asociar el socket al puerto local (%d)", ntohs(server_addr.sin_port));
    	close(socket_fd);
    	return 1;
  	}

    /* Ponemos el puerto a la escucha */
	int l = listen(socket_fd, NUM_CLIENTS);
	if (l < 0) { /* Comprobamos si se puso a la escucha el puerto local */
		printf("ERROR: error al escuchar el puerto local (%d)", ntohs(server_addr.sin_port));
		return 1;
	}
	
	system("clear");
	printf("\n");
	printf("\tServidor CONECTADO\n");
	printf("\tSOCKET ESCUCHANDO...\n");
    printf("\tPUERTO: %d\n\n", ntohs(server_addr.sin_port));

    puts("\t\nEsperando las conexiones...");
    pthread_t thread[NUM_CLIENTS];

//    for(int i = 0; i < NUM_CLIENTS; i++)
//        strcpy(s_cli[i].user, " ");

    while(1) {
        socklen_t cli_size = sizeof(cli_addr); /* Obtenemos el tamaño de la estructura en bytes (16)*/
		int conn_accept = accept(socket_fd, (struct sockaddr*)&cli_addr, &cli_size);
		if (conn_accept < 0) {
		    perror("ERROR: error al conectarse al servidor..");
		    exit(0);
		} else {
		    int id = clients;
		    s_cli[id].socket = conn_accept;
		    s_cli[id].sign_in = 0;
		    fflush(stdout);
		    clients++;
            if (pthread_create(&thread[id], NULL, connection, (void *)&id) < 0) {
                perror("ERROR: error al crear el hilo...");
            }
        }
    }
    close(socket_fd);
    return 0;
}
