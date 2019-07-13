#ifndef HEADER_H
#define _LARGEFILE_SOURCE /* or _LARGEFILE64_SOURCE or _LARGEFILE_SOURCE */

/* Esta union hay que definirla o no según el valor de los defines aqui indicados. */
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/* La union ya está definida en sys/sem.h */
#else
/* Tenemos que definir la union */
union semun 
	{ 
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
	};
#endif

#define HEADER_H
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/types.h> /* Tipos de datos ej: pthread_t */
#include <sys/fcntl.h> /* Read() y write() */
#include <sys/unistd.h>
#include <sys/socket.h> /* Funciones para gestionar los sockets tales como acept(), bind(), conect(), etc */
#include <netinet/in.h> /* Contiene las estructuras sockaddr_in y hostent */
#include <netdb.h> /* Contiene las funciones como por ej gethostbyname() */
#include <stdlib.h> /* Contiene los prototipos de funciones de C para gestión de memoria dinámica, control de procesos y otras */
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <errno.h>






typedef struct 
	{
	//int total;
	int acept;
	int hilo[3][1];
	} conexiones;

typedef struct 
	{
	int usuario[5]; // La posicion 0, es para el usuario 0. Si ese casillero esta en 0, al usuario 0 no se le enviará el contenido. Si esta en 1, si se le enviará.
	int origen; // Va el acepts del usuario que envio el mensaje.
	char contenido[512];
	} mensaje;




/* Define los colores */
#define BLANCO "\033[0m"
#define ROJO "\033[0;31m"
#define VERDE "\033[0;32m"
#define AMARILLO "\033[0;33m"
#define AZUL "\033[0;34m"
#define VIOLETA "\033[0;35m"	
#define MAGENTA "\033[0;36m"
#define CYAN "\033[0;37m"

#endif
