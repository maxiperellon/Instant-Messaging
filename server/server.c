#include "header.h"

int main(int argc, char *argv[]) 
{ 
    int sockfd=0, conx_acept=0, cont=0, l, i, b, t_struct; 
    struct sockaddr_in servaddr;
	//struct sockadrr_in servcli;
	pthread_t idHilo;

  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	
    if (sockfd < 0){ 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 

    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(SERVER_PORT); 

	t_struct =  sizeof(struct sockaddr); /* Obtenemos el tamaño de la estructura en bites */

	b = bind(sockfd, (struct sockaddr *)&servaddr, t_struct); /* Asociamos el socket a un puerto local */

	if (b < 0) { /* Comprobamos si se asocio el socket a un puerto local */
    	printf("ERROR: error de bind");
    	return 1;
  	}
  
	l = listen(sockfd, NUM_CLIENTES); /* Ponemos el puerto a la escucha */

	if (l < 0) { /* Comprobamos si se puso a la escucha el puerto local */
		printf("ERROR: error de listen");
		return 1;
	}
	
	system("clear");
	printf("\n\n");	
	printf("\tServidor CONECTADO\n");
	printf("\tSOCKET %d ESCUCHANDO ...\n\n\n", SERVER_PORT);


	while(1)
	{
		conx_acept=accept(sockfd, (struct sockaddr*)&servaddr, &t_struct);
		if(conx_acept>0)
		{ 			

		 for(i=0; i < NUM_CLIENTES; i++) /* Agregar Cliente */
			{
				cont++;

				if(cont > NUM_CLIENTES)
				{
					printf("\tERROR: Supero la cantidad maxima de clientes\n");
					close(conx_acept);
				}
				
				
			}
		}
	}
				
				
				
   
} 
