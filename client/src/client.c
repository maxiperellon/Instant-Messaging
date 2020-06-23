#include "header.h"

int main(int argc, char *argv[]) {

    /* Asociamos el socket a un puerto local */
    struct sockaddr_in server_addr;
    socklen_t server_size = sizeof(server_addr);
    memset(&server_addr, 0, server_size);
    server_addr.sin_family = AF_INET; //tipo de conexión (por red o interna)
    server_addr.sin_port = htons(SERVER_PORT); //es el numero que le corresponde al puerto

    pthread_t send_thread, recv_thread;

    //CREACION DEL SOCKET

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("ERROR: error al crear el socket..\n");
        return 1;
    }
    //connect solicita la conexion con el servidor. 0 --> establece la conexion si no larga error
    if (connect(sock, (struct sockaddr*)&server_addr, server_size) < 0) {
        perror("ERROR: error al conectar con el servidor..\n");
        exit(0);
    } else {
        if (pthread_create(&send_thread, NULL, send_msg, (void*)&sock) < 0) {
            perror("ERROR: error al crear el hilo para enviar..");
            exit(0);
        }
        if (pthread_create(&recv_thread, NULL, recv_msg, (void*)&sock) < 0) {
            perror("ERROR: error al crear el hilo para recibir..");
            exit(0);
        }
        pthread_join(send_thread, NULL);
        pthread_join(recv_thread, NULL);
    }
    close(sock);
    return 0;
}