/* servidor.c
 * Servidor que permite conectarse simultáneamente a 3 usuarios  */

#include "header.h"

#define BUF_SIZE 512 /* Definimos el tamaño del Buffer */
#define QUEQUE_SIZE 10

/*Estas son variables globales. Por este motivo, se le asignan semaforos.*/
conexiones conx; /* SEM 0*/
mensaje msj; /* SEM 1 */

/* Hilo que atiene a los usuarios, recive el nick y el pass y procesa lo que envian */
void *conectar(void); 
void *conectar2(void); 
void *conectar3(void);

/* Hilo que se encarga de enviar buffers de texto a los usuarios conectados. */
void *mandar(void);

int semaforo (int sem, int indice, int ope);

//--------------------------------------------------------------------------------------------------
char* reemplazarMalaPalabra (char* cadena);
char* buscarPosicionCadena(char* cadena, char* caracteres);
char* guardarConversacion (char* cadena);

//--------------------------------------------------------------------------------------------------

void main(int argc,char **argv)
	{	
	int SERVER_PORT;
	int s, sa, l, b,tam, iniciar=1, pid, i, i2, a=0, tiempo=0, aux;
	char buf[BUF_SIZE];		
	
	/* Variables semáforos */
	key_t clave_sem;
	int id_sem;
	struct sembuf op;
	union semun arg;

	if (argc !=2) /* Uso */
		{
		printf("Uso: cliente <Puerto>\n");
		exit(0);
		}
			
	pthread_t idHilo; /* Se crea un hilo */	
	
 	SERVER_PORT=atoi(argv[1]); /* Pasa el puerto a entero y lo guarda en SERVER_PORT */
 	/* atoi: string to int*/
	
	struct sockaddr_in channel; /* Creamos una estructura para el socket */

	/* Construcción de la estructura para enlazar el socket: 
	 * "sin_family" es el tipo de conexión: por red (AF_INET) o interna (AF_UNIX).
	 * "sin_addr.s_addr" es la dirección del cliente al que queremos atender. Colocando en ese campo el valor 
	 * INADDR_ANY  atenderemos a cualquier cliente.
	 * "sin_port" es el número correspondiente al puerto (en formato de red). 
	 */		

	channel.sin_family = AF_INET;
	channel.sin_addr.s_addr = htonl(INADDR_ANY); /* Traducimos la IP (host to network long)*/
	channel.sin_port = htons(SERVER_PORT); /* Ponemos el puerto en formato de red (host to network short) */
	memset (&(channel.sin_zero),'\0',8); /* Cambiamos los primeros 8 bytes a 0 */
	 
	/* Función socket(): Crear un descriptor para antender conexiones.
	 * int socket(int domain, int type, int protocol); 
	 * El primer parámetro es AF_INET o AF_UNIX para indicar si los clientes pueden estar en otros ordenadores distintos del servidor
	 * o van a correr en el mismo ordenador.
	 * El segundo parámetro indica si el socket es orientado a conexión (SOCK_STREAM) o no lo es (SOCK_DGRAM).
	 * El tercer parámetro indica el protocolo a emplear. Habitualmente se pone 0. 
	 */

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); /* Crea el socket y nos devuelve un descriptor de fichero */
	
	if ( s < 0 ) /* Comprobamos si se creo el socket */
		{
		printf("ERROR: error de socket\n");
		exit(0);
		}

	tam = sizeof(struct sockaddr); /* Obtenemos el tamaño de la estructura en bites */

	/* Función bind(): Asociar el socket a un puerto local
	 * int bind(int sockfd, struct sockaddr *my_addr, int addrlen); 
	 * El primer parámetro es descriptor del socket que hemos abierto.
	 * El segundo parámetro indica el puntero a la estructura "sockaddr_in" con los datos indicados anteriormente.
 	 * (a la hora de pasar el parámetro, debemos hacer un "cast" al tipo "struct sockaddr")
	 * El tercer parámetro indica la longitud de la estructura. 
	 */

	b = bind(s, (struct sockaddr *) &channel, tam); /* Asociamos el socket a un puerto local */

	if ( b < 0 ) /* Comprobamos si se asocio el socket a un puerto local */
		{
		printf("ERROR: error de bind\n");
		exit(0);
		}
	
	/* Función listen(): Indicar al sistema que comience a antender llamadas
	 * int listen (int socket, int n);
	 * El primer parámetro es descriptor del socket que hemos abierto.
	 * El segundo parámetro indica elnúmero máximo de clientes encolados.
	 */

	l = listen(s, QUEQUE_SIZE); /* Ponemos el puerto a la escucha */

	if ( l < 0 ) /* Comprobamos si se puso a la escucha el puerto local */
		{
		printf("ERROR: error de listen\n");
		exit(0);
		}

	system("clear");
	printf("\n\n");	
	printf("\tServidor CONECTADO\n");
	printf("\tSOCKET %d ESCUCHANDO ...\n\n\n", SERVER_PORT);
	
	/* Obtener una clave
	 * key_t  ftok(char *, int);
	 * Primer parámetro: Lugar donde se va a guardar la clave
	 * Segundo parámetro: Un número entero cualquiera. 
	 * (Todos los procesos que quieran compartir el semáforo, deben suministrar el mismo fichero y el mismo entero) 
	 * Devuelve: clave del recurso compartido
	 */
	 	
	clave_sem= ftok (".", 22);	
	
	/* Obtener un array de semáforo:
	 * int semget (key_t, int, int);
	 * Primer argumento: clave del semáforo (obtenida del tfork())
	 * Segundo argumento: número de semáforos
	 * Tercer argumento: flags
	 *	0666: Permisos (siempre poner 0 al inicio para que el sist detecte que esta en octal)
	 *	IPC_CREAT: Crea el semáforo si no ha sido creado aún.
	 * Devuelve: ID del semáforo.
	 */
	 
	 
	if(id_sem=semget (clave_sem, 6 , 0666 | IPC_CREAT | IPC_EXCL)==-1)
		{
		if(errno==EEXIST)
			{
			id_sem=semget (clave_sem, 6 , 0);
			if(id_sem==-1)
				{
				printf("ERROR: No se pudo obtener array del semáforo.\n");
				exit(1);
				}
			}
		}
		
	/* Inicializar el semáforo:
	 * int semctl (int, int, int, int);
	 * Primer argumento: Identificador del array del semáforo (obtenida del semget()).
	 * Segundo argumento: Índice del semáforo que queremos inicializar dentro del array de semáforos obtenido.
	 * Tercer argumento: ¿Qué queremos hacer con el semáforo? Para inicializar ponemos SETVAL.
	 * Cuarto argumento: 1 si queremos el semáforo en VERDE o un -1 si lo queremos en ROJO.
	 */

	if(semctl (id_sem, 0, SETVAL, 1)==-1){
	printf("(0) ERROR: No se pudo inicializar el semáforo.\n"); exit(1);}
	if(semctl (id_sem, 1, SETVAL, 1)==-1){
	printf("(1) ERROR: No se pudo inicializar el semáforo.\n"); exit(1);}
	if(semctl (id_sem, 2, SETVAL, 1)==-1){
	printf("(2) ERROR: No se pudo inicializar el semáforo.\n"); exit(1);}
	if(semctl (id_sem, 3, SETVAL, 1)==-1){
	printf("(3) ERROR: No se pudo inicializar el semáforo.\n"); exit(1);}
	if(semctl (id_sem, 4, SETVAL, 1)==-1){
	printf("(4) ERROR: No se pudo inicializar el semáforo.\n"); exit(1);}	
	if(semctl (id_sem, 5, SETVAL, 1)==-1){
	printf("(4) ERROR: No se pudo inicializar el semáforo.\n"); exit(1);}	
	
	/* Creamos los Hilos con la función pthread_create(pthread_t *, pthread_attr_t *, void *(*)(void *), void *);
	 * El primer argumento es el puntero que identifica al hilo
	 * El segundo argumento son los atributos de la creación de hilo por ejemplo el de la prioridad (NULL= Atributos por defecto)
	 * El tercer argumento es la función que ejecutara el hilo
	 * El cuarto argumento es los parámetros que le pasamos a esa función	
	 */

	if((pthread_create (&idHilo, NULL, conectar, NULL))!=0) /* Creamos un hilo */
		{
		puts("Error HILO: conectar");
		}

	if((pthread_create (&idHilo, NULL, conectar2, NULL))!=0)
		{
		puts("Error HILO: conectar2");
		}

	if((pthread_create (&idHilo, NULL, conectar3, NULL))!=0)
		{
		puts("Error HILO: conectar3");
		}

	if((pthread_create (&idHilo, NULL, mandar, NULL))!=0)
		{
		puts("Error HILO: mandar");
		}


	/* Función accept: aceptamos conexiones.
	 * int accept (int socket, struct sockaddr *addr, socklen_t *length_ptr);
	 * El primer parámetro es descriptor del socket que hemos abierto.
	 * El segundo argumento es un puntero a estructura sockaddr. Esta estructura contendrá la dirección y demás datos del 
	 * ordenador cliente que se ha conectado a nosotros.
	 * El tercer parámetro indica la longitud de la estructura. 	
	 */		

	while(1)
		{
		conx.acept=accept(s, (struct sockaddr*)&channel, &tam);		
		if(conx.acept>0)
			{
			/* Atiende las conexiones (máximo 3 simultanas):
			 * Si encuentra un hilo "libre" lo utiliza.
			 */					
			for(i=0; i<3; i++) /* Analiza los hilos */
				{
				/* Cuando el valor es 0 el hilo esta libre si no el valor es el resultado del acept */
				
				/* [numero!=0 -> ocupado, 0=libre]
				   [1=ocupado, 0=libre]
				   [1=ocupado, 0=libre] 
				*/
				
				if(conx.hilo[i][0]==0) 	
					{
					if(semaforo(id_sem, 0, -1)==-1){perror ("Error 0");}
					conx.hilo[i][0]=conx.acept;					
					conx.acept=0;	
					if(semaforo(id_sem, 0, 1)==-1){perror ("Error 0.0");}				
					i=3;		
					}
				}			
			}
		}
	close(s); /* Cerramos el socket */
	}	

void *conectar (void)	
	{
	char buf[BUF_SIZE];
	char nick[BUF_SIZE];
	int acept=0;
	
  	int num_fields;
 	int i=0, a=0;

 	key_t clave_sem;
 	int id_sem;
 	
 	clave_sem= ftok (".", 22);	
	id_sem= semget (clave_sem, 6 ,0); 

	while(1)
		{
		while(conx.hilo[0][0]==0) {} /* Espera hasta que se le asigna un usuario */

/*aca deberia haber un semaforo*/
/*siempre conviene trabajar con variables locales (como acept) y no trabajar con variables globales, ya que cada vez que accedemos a ellAS necesitamos un semaforo*/
		acept=conx.hilo[0][0];

		buf[0]='\0';
	
		/* Función read: "leer" lo que nos manda el usuario.
		 * int read(int acept, void *buffer, int nbyte );
		 * El primer parámetro es descriptor del fichero / socket del que se quiere leer.
		 * El segundo argumento es un puntero a char donde se almacenarán los datos leídos 
		 * El tercer parámetro indica el número de caracteres que se quieren leer.	
		 */		

		read(acept, buf, BUF_SIZE); /* Lee el nick */
		
		nick[0]='\0';
				
		while(buf[i]!='\0')
			{
			nick[i]=buf[i];
			i++;
			}
			
		nick[i]='\0';	
		
		a=0;
		i=0;
		
		printf("Entró %s\n", nick);		
	 	
		 	/* Función write: Enviar un mensaje al usuario.
		 	 * int write( int acept, void *buffer, int nbyte);
			 * El primer parámetro es descriptor del fichero / socket del que se quiere leer.
			 * El segundo argumento es un puntero a char donde se almacenarán los datos que deseamos enviar.
			 * El tercer parámetro indica el número de caracteres que se quieren leer.	
			 */			 

			/* Envia un mensaje de bienvenida */
			write(acept, "Bienvenido al CHAT.", BUF_SIZE);
			
			/* Comunica a los demás usuarios que se acaba de conectar alguien */
			
			if(semaforo(id_sem, 1, -1)==-1){perror ("Error 1");}
			
			msj.contenido[0]='\0';
			
			strcat(msj.contenido, VIOLETA);
			strcat(msj.contenido, " --> "); 
			strcat(msj.contenido, VERDE);
			strcat(msj.contenido,"Entro [");	
			strcat(msj.contenido, AMARILLO);	
			strcat(msj.contenido, nick);	
			strcat(msj.contenido, VERDE);
			strcat(msj.contenido, "].");
			strcat(msj.contenido, BLANCO);
	
			/* Levanta la bandera de los otros dos usuarios y no la de este */
			if(conx.hilo[1][0]!=0){
			msj.usuario[1]=1;}
			if(conx.hilo[2][0]!=0){
			msj.usuario[2]=1;}				
						
			if(semaforo(id_sem, 1, 1)==-1){perror ("Error 0");}
		
			while(read(acept, buf, BUF_SIZE)>0) /*  Mientras el usuario este conectado que lea lo que se le envia */
				{			
				if(semaforo(id_sem, 1, -1)==-1){perror ("Error 0");}			
				msj.contenido[0]='\0';
				//strcpy(msj.contenido, ROJO);
				strcat(msj.contenido, nick);
				strcat(msj.contenido, VERDE);
				strcat(msj.contenido, ": ");
				strcat(msj.contenido, BLANCO);
				strcat(msj.contenido, buf);

				printf("%s\n", msj.contenido);
				
						
				if(conx.hilo[1][0]!=0){
				msj.usuario[1]=1;}
				if(conx.hilo[2][0]!=0){
				msj.usuario[2]=1;}
				if(semaforo(id_sem, 1, 1)==-1){perror ("Error 0");}

			}
				
			
			/* Comunicamos a los demás usuarios que se acaba de desconectar un cliente */
			if(semaforo(id_sem, 1, -1)==-1){perror ("Error 0");}
			msj.contenido[0]='\0';
			strcat(msj.contenido, VIOLETA);
			strcat(msj.contenido, " <-- "); 
			strcat(msj.contenido, VERDE);
			strcat(msj.contenido,"Salio [");	
			strcat(msj.contenido, AMARILLO);	
			strcat(msj.contenido, nick);	
			strcat(msj.contenido, VERDE);
			strcat(msj.contenido, "].");
			strcat(msj.contenido, BLANCO);
			if(conx.hilo[1][0]!=0){
			msj.usuario[1]=1;}
			if(conx.hilo[2][0]!=0){
			msj.usuario[2]=1;}
			if(semaforo(id_sem, 1, 1)==-1){perror ("Error 0");}
	
			printf("Se desconectó %s.\n", nick);			

		/* Liberamos el hilo */
		if(semaforo(id_sem, 0, -1)==-1){perror ("Error 0");}
		conx.hilo[0][0]=0; 
		if(semaforo(id_sem, 0, 1)==-1){perror ("Error 0");}
		}
	}


void *conectar2 (void)	
	{
	char buf[BUF_SIZE];
	char nick[BUF_SIZE];
	int acept=0;
	
  	int num_fields;
 	int i=0, a=0;

 	key_t clave_sem;
 	int id_sem;
 	
 	clave_sem= ftok (".", 22);	
	id_sem= semget (clave_sem, 6 ,0); 

	while(1)
		{
		while(conx.hilo[1][0]==0) {} /* Espera hasta que se le asigna un usuario */

/*aca deberia haber un semaforo*/
/*siempre conviene trabajar con variables locales (como acept) y no trabajar con variables globales, ya que cada vez que accedemos a ellAS necesitamos un semaforo*/
		acept=conx.hilo[1][0];

		buf[0]='\0';
	
		/* Función read: "leer" lo que nos manda el usuario.
		 * int read(int acept, void *buffer, int nbyte );
		 * El primer parámetro es descriptor del fichero / socket del que se quiere leer.
		 * El segundo argumento es un puntero a char donde se almacenarán los datos leídos 
		 * El tercer parámetro indica el número de caracteres que se quieren leer.	
		 */		

		read(acept, buf, BUF_SIZE); /* Lee el nick */
		
		nick[0]='\0';
				
		while(buf[i]!='\0')
			{
			nick[i]=buf[i];
			i++;
			}
			
		nick[i]='\0';	
		
		a=0;
		i=0;
		
		printf("Entró %s\n", nick);		
	 	
		 	/* Función write: Enviar un mensaje al usuario.
		 	 * int write( int acept, void *buffer, int nbyte);
			 * El primer parámetro es descriptor del fichero / socket del que se quiere leer.
			 * El segundo argumento es un puntero a char donde se almacenarán los datos que deseamos enviar.
			 * El tercer parámetro indica el número de caracteres que se quieren leer.	
			 */			 

			/* Envia un mensaje de bienvenida */
			write(acept, "Bienvenido al CHAT.", BUF_SIZE);
			
			/* Comunica a los demás usuarios que se acaba de conectar alguien */
			
			if(semaforo(id_sem, 1, -1)==-1){perror ("Error 1");}
			
			msj.contenido[0]='\0';
			
			strcat(msj.contenido, VIOLETA);
			strcat(msj.contenido, " --> "); 
			strcat(msj.contenido, VERDE);
			strcat(msj.contenido,"Entro [");	
			strcat(msj.contenido, AMARILLO);	
			strcat(msj.contenido, nick);	
			strcat(msj.contenido, VERDE);
			strcat(msj.contenido, "].");
			strcat(msj.contenido, BLANCO);
	
			/* Levanta la bandera de los otros dos usuarios y no la de este */
			if(conx.hilo[0][0]!=0){
			msj.usuario[0]=1;}
			if(conx.hilo[2][0]!=0){
			msj.usuario[2]=1;}				
						
			if(semaforo(id_sem, 1, 1)==-1){perror ("Error 0");}
		
			while(read(acept, buf, BUF_SIZE)>0) /*  Mientras el usuario este conectado que lea lo que se le envia */
				{			
				if(semaforo(id_sem, 1, -1)==-1){perror ("Error 0");}			
				msj.contenido[0]='\0';
				//strcpy(msj.contenido, ROJO);
				strcat(msj.contenido, nick);
				strcat(msj.contenido, VERDE);
				strcat(msj.contenido, ": ");
				strcat(msj.contenido, BLANCO);
				strcat(msj.contenido, buf);	
				printf("%s\n", msj.contenido);
						
				if(conx.hilo[0][0]!=0){
				msj.usuario[0]=1;}
				if(conx.hilo[2][0]!=0){
				msj.usuario[2]=1;}
				if(semaforo(id_sem, 1, 1)==-1){perror ("Error 0");}

			}
				
			
			/* Comunicamos a los demás usuarios que se acaba de desconectar un cliente */
			if(semaforo(id_sem, 1, -1)==-1){perror ("Error 0");}
			msj.contenido[0]='\0';
			strcat(msj.contenido, VIOLETA);
			strcat(msj.contenido, " <-- "); 
			strcat(msj.contenido, VERDE);
			strcat(msj.contenido,"Salio [");	
			strcat(msj.contenido, AMARILLO);	
			strcat(msj.contenido, nick);	
			strcat(msj.contenido, VERDE);
			strcat(msj.contenido, "].");
			strcat(msj.contenido, BLANCO);
			if(conx.hilo[0][0]!=0){
			msj.usuario[0]=1;}
			if(conx.hilo[2][0]!=0){
			msj.usuario[2]=1;}
			if(semaforo(id_sem, 1, 1)==-1){perror ("Error 0");}
	
			printf("Se desconectó %s.\n", nick);			

		/* Liberamos el hilo */
		if(semaforo(id_sem, 0, -1)==-1){perror ("Error 0");}
		conx.hilo[1][0]=0; 
		if(semaforo(id_sem, 0, 1)==-1){perror ("Error 0");}
		}
	}
	
	void *conectar3 (void)	
	{
	char buf[BUF_SIZE];
	char nick[BUF_SIZE];
	int acept=0;
	
  	int num_fields;
 	int i=0, a=0;

 	key_t clave_sem;
 	int id_sem;
 	
 	clave_sem= ftok (".", 22);	
	id_sem= semget (clave_sem, 6 ,0); 

	while(1)
		{
		while(conx.hilo[2][0]==0) {} /* Espera hasta que se le asigna un usuario */

/*aca deberia haber un semaforo*/
/*siempre conviene trabajar con variables locales (como acept) y no trabajar con variables globales, ya que cada vez que accedemos a ellAS necesitamos un semaforo*/
		acept=conx.hilo[2][0];

		buf[0]='\0';
	
		/* Función read: "leer" lo que nos manda el usuario.
		 * int read(int acept, void *buffer, int nbyte );
		 * El primer parámetro es descriptor del fichero / socket del que se quiere leer.
		 * El segundo argumento es un puntero a char donde se almacenarán los datos leídos 
		 * El tercer parámetro indica el número de caracteres que se quieren leer.	
		 */		

		read(acept, buf, BUF_SIZE); /* Lee el nick */
		
		nick[0]='\0';
				
		while(buf[i]!='\0')
			{
			nick[i]=buf[i];
			i++;
			}
			
		nick[i]='\0';	
		
		a=0;
		i=0;
		
		printf("Entró %s\n", nick);		
	 	
		 	/* Función write: Enviar un mensaje al usuario.
		 	 * int write( int acept, void *buffer, int nbyte);
			 * El primer parámetro es descriptor del fichero / socket del que se quiere leer.
			 * El segundo argumento es un puntero a char donde se almacenarán los datos que deseamos enviar.
			 * El tercer parámetro indica el número de caracteres que se quieren leer.	
			 */			 

			/* Envia un mensaje de bienvenida */
			write(acept, "Bienvenido al CHAT.", BUF_SIZE);
			
			/* Comunica a los demás usuarios que se acaba de conectar alguien */
			
			if(semaforo(id_sem, 1, -1)==-1){perror ("Error 1");}
			
			msj.contenido[0]='\0';
			
			strcat(msj.contenido, VIOLETA);
			strcat(msj.contenido, " --> "); 
			strcat(msj.contenido, VERDE);
			strcat(msj.contenido,"Entro [");	
			strcat(msj.contenido, AMARILLO);	
			strcat(msj.contenido, nick);	
			strcat(msj.contenido, VERDE);
			strcat(msj.contenido, "].");
			strcat(msj.contenido, BLANCO);
	
			/* Levanta la bandera de los otros dos usuarios y no la de este */
			if(conx.hilo[1][0]!=0){
			msj.usuario[1]=1;}
			if(conx.hilo[0][0]!=0){
			msj.usuario[0]=1;}				
						
			if(semaforo(id_sem, 1, 1)==-1){perror ("Error 0");}
		
			while(read(acept, buf, BUF_SIZE)>0) /*  Mientras el usuario este conectado que lea lo que se le envia */
				{			
				if(semaforo(id_sem, 1, -1)==-1){perror ("Error 0");}			
				msj.contenido[0]='\0';
				//strcpy(msj.contenido, ROJO);
				strcat(msj.contenido, nick);
				strcat(msj.contenido, VERDE);
				strcat(msj.contenido, ": ");
				strcat(msj.contenido, BLANCO);
				strcat(msj.contenido, buf);	
				printf("%s\n", msj.contenido);
						
				if(conx.hilo[1][0]!=0){
				msj.usuario[1]=1;}
				if(conx.hilo[0][0]!=0){
				msj.usuario[0]=1;}
				if(semaforo(id_sem, 1, 1)==-1){perror ("Error 0");}

			}
				
			
			/* Comunicamos a los demás usuarios que se acaba de desconectar un cliente */
			if(semaforo(id_sem, 1, -1)==-1){perror ("Error 0");}
			msj.contenido[0]='\0';
			strcat(msj.contenido, VIOLETA);
			strcat(msj.contenido, " <-- "); 
			strcat(msj.contenido, VERDE);
			strcat(msj.contenido,"Salio [");	
			strcat(msj.contenido, AMARILLO);	
			strcat(msj.contenido, nick);	
			strcat(msj.contenido, VERDE);
			strcat(msj.contenido, "].");
			strcat(msj.contenido, BLANCO);
			if(conx.hilo[1][0]!=0){
			msj.usuario[1]=1;}
			if(conx.hilo[0][0]!=0){
			msj.usuario[0]=1;}
			if(semaforo(id_sem, 1, 1)==-1){perror ("Error 0");}
	
			printf("Se desconectó %s.\n", nick);			

		/* Liberamos el hilo */
		if(semaforo(id_sem, 0, -1)==-1){perror ("Error 0");}
		conx.hilo[2][0]=0; 
		if(semaforo(id_sem, 0, 1)==-1){perror ("Error 0");}
		}
	}
	
void *mandar (void)	
	{
	char buf2[8192];
	char buf1[8192];
	char nick[300];
	int pid=0;
	
	key_t clave_sem;
 	int id_sem;
 	
 	clave_sem= ftok (".", 22);	
	id_sem= semget (clave_sem, 6 ,0); 

	while(1)
		{
		if(semaforo(id_sem, 1, -1)==-1){perror ("Error 0");}
		if(msj.usuario[0]==1 && conx.hilo[0][0]!=0)			
			{
			//printf("Entrooooo");
			write(conx.hilo[0][0], reemplazarMalaPalabra(msj.contenido), BUF_SIZE);
			msj.usuario[0]=0;
			
			}
		if(msj.usuario[1]==1 && conx.hilo[1][0]!=0)			
			{
			write(conx.hilo[1][0], reemplazarMalaPalabra(msj.contenido), BUF_SIZE);
			msj.usuario[1]=0;
			
			}
		if(msj.usuario[2]==1 && conx.hilo[2][0]!=0)			
			{
			write(conx.hilo[2][0], reemplazarMalaPalabra(msj.contenido), BUF_SIZE);
			msj.usuario[2]=0;
			
			}
		if(semaforo(id_sem, 1, 1)==-1){perror ("Error 0");}
		}
	}

int semaforo (int sem, int indice, int ope)
	{
	struct sembuf sb;

	sb.sem_num = indice; /* Índice del array del semáforo sobre el que queremos actuar */
	sb.sem_op = ope; /* -1 Para acceder a un recurso, 1 cuando termina de usar el recurso */
	sb.sem_flg = SEM_UNDO; /* Flags */

	/* Usar semáforos:
	 * int semop (int, strcut sembuf *, size_t);
	 * Primer argumento: Identificador del array del semáforo ( obtenida del semget() ).
	 * Segundo argumento: Estructura sembuf;
	 * Tercer argumento: número de índices.
	 */
	 	
	return ( semop(sem, &sb, 1) ); /* Devuelve -1 si hay error */
	}
		
//--------------------------------------------------------------
char* reemplazarMalaPalabra (char* cadena){
	FILE *archivo;
	archivo = fopen("diccionario", "r");
	
	char* cadena2;
	char* caracteres;
	
	cadena2=(char*)malloc(BUF_SIZE*sizeof(char));
	caracteres=(char*)malloc(BUF_SIZE*sizeof(char));
	
	strcpy(cadena2, cadena);
	
	while (feof(archivo) == 0)
 		{
 		fgets(caracteres,50,archivo);
 		
 		caracteres[strlen(caracteres)-1]='\0';
  		 
 		if(strstr(cadena2, caracteres)!=NULL){
 			strcpy(cadena2, buscarPosicionCadena(cadena2, caracteres));
			}
			
		caracteres[0]='\n';
 		}
	
	fclose(archivo);
 time_t t;
  struct tm *tm;
  char fechayhora[100];

  t=time(NULL);
  tm=localtime(&t);
  strftime(fechayhora, 100, "%d/%m/%Y %H:%M:%S", tm);
strcat(cadena2," ");
strcat(cadena2,fechayhora);
 
	guardarConversacion (cadena2);
	return cadena2;
}

char* buscarPosicionCadena(char* cadena, char* caracteres){
	int i=0, e=0, coincidencias=0;
	char* cadena2;
	
	cadena2=(char*)malloc(BUF_SIZE*sizeof(char));
	
	strcpy(cadena2, cadena);
	
	for(i=0; i<strlen(cadena2); i++){
		if(cadena[i] == caracteres[0]){
			coincidencias++;
			for(e=1; e<strlen(caracteres); e++){
				if(cadena[i+e] == caracteres[e]){
					coincidencias++;
				}	
			}
		
		if (coincidencias == strlen(caracteres)){
			for(e=1; e<strlen(caracteres); e++){
				cadena2[i+e] = '*';
			}
		}
		coincidencias  = 0;
		
		}
	}
	
			return cadena2;
}

//-------------------------------------------------
char* guardarConversacion (char* cadena){
	
		char cadena2[128];
	
 	FILE *fp;
 
 	
 
 	fp = fopen ( "conversacion.txt", "a+" );
 	

 	
	strcpy(cadena2,cadena);
	strcat(cadena2,"\n");
 	fputs( cadena2, fp );
 
 	fclose ( fp );
 
 	return cadena;
}
