#include "header.h"

int main(int argc, char *argv[]) 
{ 
    int cont=0, l, i, b;
    struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	pthread_t thread;


    //CREACION DEL SOCKET
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
    if (sockfd < 0){
        printf("Error al crear el socket...\n");
        exit(0); 
    } 

    serv_addr.sin_family = AF_INET; //tipo de conexión (por red o interna)
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //es la direccion del cliente al que se quiere atender -> INADDR_ANY (valor con el que se atiende a cualquier cliente)
    serv_addr.sin_port = htons(SERVER_PORT); //es el numero que le corresponde al puerto

	socklen_t size_serv =  sizeof(serv_addr); /* Obtenemos el tamaño de la estructura en bytes (16)*/

    /* Asociamos el socket a un puerto local */
	b = bind(sockfd, (struct sockaddr *)&serv_addr, size_serv);

	if (b < 0) { /* Comprobamos si se asocio el socket a un puerto local */
    	printf("ERROR: error de bind");
    	return 1;
  	}

    /* Ponemos el puerto a la escucha */
	l = listen(sockfd, NUM_CLIENTES);

	if (l < 0) { /* Comprobamos si se puso a la escucha el puerto local */
		printf("ERROR: error de listen");
		return 1;
	}
	
	system("clear");
	printf("\n\n");	
	printf("\tServidor CONECTADO\n");
	printf("\tSOCKET %d ESCUCHANDO ...\n\n\n", SERVER_PORT);


	while(1) {
	    socklen_t size_cli = sizeof(cli_addr);
		int conn_accept = accept(sockfd, (struct sockaddr*)&cli_addr, &size_cli);
		if(conn_accept>0) {
		    for(i=0; i < NUM_CLIENTES; i++) { /* Agregar Cliente */
				cont++;
				if(cont > NUM_CLIENTES) {
					printf("\tERROR: Supero la cantidad maxima de clientes\n");
					close(conn_accept);
				}
			}
		}
	}

} 
