#include "header.h"
#include "semaforos.h"
	//#define SERVER_PORT 25028 //puerto
	#define BUF_SIZE 512 //tamaño buffer

int c, s; 
int error[2]; // error[0] cuando el servidor cierra la conexion | error[1] cuando el usuario ingresa !salir
void *enviar (void);
void *recibir (void);


int main(int argc,char **argv)
{

	int SERVER_PORT;
	int pid;
	char buf[BUF_SIZE], buf2[BUF_SIZE], cadenax[100];

	key_t clave;
	int id_memoria;
	int *Memoriac;

	struct hostent *h;
	struct sockaddr_in channel;//estructura

	if (argc !=4)//uso
		{
		printf("Uso: ./Trivia <IP_servidor> <Puerto> <Nick>\n");
		exit(0);
		}

	SERVER_PORT=atoi(argv[2]);

	// toma direccion del host
	h = gethostbyname(argv[1]);
	
	if (!h)//error de host
	{
		printf("ERROR: fallo al intentar resolver IP del servidor.\n");
		exit(0);
	}
	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//crea el socket y nos devuelve un descriptor de fichero

	if (s < 0)//error en creacion de socket
	{
		printf("ERROR: error el tratar de abrir el socket.\n");
		exit(0);
	}
		
	channel.sin_family= AF_INET;
	memcpy(&channel.sin_addr.s_addr, h->h_addr, h->h_length);
	channel.sin_port = htons(SERVER_PORT);
	memset (&(channel.sin_zero),'\0',8);//ponemos 8 bytes a 0

	c = connect(s, (struct sockaddr *) &channel, sizeof(channel));//conexion: 
			//socket, estrutura de socket diseñada para protocolos de internet, tamaño de estructura.


	if (c < 0)//error de connect
		{
		printf("ERROR: error el tratar de abrir un conexión.\n");
		exit(0);
		}
		
	system("clear");

	sleep(1); // Duerme un segundo y despues manda el nick...

	//printf("NICK %s.\n", argv[3]);

	write(s, argv[3], BUF_SIZE);

	sleep(1);

	read(s, buf2, BUF_SIZE);
		
	if (strncmp(buf2, "ERROR", 5)==0)
		{
		printf("%s\n\n", buf2);
		exit (0);
		}

	printf("%s\n", buf2);	

	pthread_t idHilo; // Se crea un hilo	

	if((pthread_create (&idHilo, NULL, enviar, NULL))!=0)
		{
		puts("Error 1");
		}
	if((pthread_create (&idHilo, NULL, recibir, NULL))!=0)
		{
		puts("Error 1");
		}

	while(error[0]==0 && error[1]==0)
		{}

	return 0;

	}

void *enviar (void)
	{
	char buf[8192];

	while(1)
		{			
		buf[0]='\0';
		gets(buf);
		if(strcmp(buf, "!salir")!=0)
			{
			write(s, buf, BUF_SIZE);
			}
		else
			{
			error[1]=1;
			}
		}

	close (c);//cerramos socket
	}

void *recibir (void)	
	{
	char buf[8192];
	
	
	while(1)
		{
		buf[0]='\0';
		if(read(s, buf, BUF_SIZE)>0)
			{			
			printf("%s\n", buf);	
			}
		else
			{
			printf("El servidor cerro la conexion.\n");
			error[0]=1;	
			exit(0);		
			}
		}

	close (c);//cerramos socket

	}	



