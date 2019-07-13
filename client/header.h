#ifndef HEADER_H
    #define HEADER_H
        #include <stdio.h>
        #include <string.h>
        #include <sys/stat.h>
        #include <sys/types.h> /* Tipos de datos ej: pthread_t */
        #include <fcntl.h> /* Read() y write() */
        #include <unistd.h>
        #include <sys/socket.h> /* Funciones para gestionar los sockets tales como acept(), bind(), conect(), etc */
        #include <netinet/in.h> /* Contiene las estructuras sockaddr_in y hostent */
        #include <netdb.h> /* Contiene las funciones como por ej gethostbyname() */
        #include <stdlib.h> /* Contiene los prototipos de funciones de C para gestión de memoria dinámica, control de procesos y otras */
//#include <signal.h>
//#include <sys/shm.h>
//#include <sys/ipc.h>
//#include <sys/sem.h>

        typedef struct  /* Crea una variable "pregunta" que contiene para almacenar: el núemro de pregunta, la pregunta, la respuesta y las ayudas. */
	        {
	        int num;
	        char cat[30];
	        char preg[300];
	        char resp[300];
	        char ayuda1[300];
	        char ayuda2[300];
	        char ayuda3[300];
	        char ayuda4[300];
	        } pregunta;

        typedef struct 
	        {
	        int total;
	        int acept[10];
	        } conexiones;

        typedef struct 
	        {
	        int usuario[5];
	        char contenido[10000];
	        } mensaje;

#endif
