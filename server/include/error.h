
#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>
#include <stdio.h>

void error_socket() {
    perror("ERROR: error al crear el socket..\n");
    exit(1);
}

void error_bind(int port, int sock) {
    printf("ERROR: error al asociar el socket al puerto local (%d)", port);
    close(sock);
    exit(1);
}

void error_listen(int port) {
    printf("ERROR: error al escuchar el puerto local (%d)", port);
    exit(1);
}

void error_conn_accept() {
    perror("ERROR: error al conectarse al servidor..");
    exit(1);
}

void error_thread_create() {
    perror("ERROR: error al crear el hilo...");
    exit(1);
}

#endif //ERROR_H
