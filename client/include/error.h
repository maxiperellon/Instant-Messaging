
#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>
#include <stdio.h>

void error_socket() {
    perror("ERROR: error al crear el socket..\n");
    exit(1);
}

void error_connect_server() {
    perror("ERROR: error al querer conectarse con el servidor..");
    exit(1);
}

void error_send_thread_create() {
    perror("ERROR: error al crear el hilo para enviar..");
    exit(1);
}

void error_recv_thread_create() {
    perror("ERROR: error al crear el hilo para recibir..");
    exit(1);
}

#endif //ERROR_H
