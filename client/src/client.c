#include "header.h"
#include "error.h"

int main(int argc, char *argv[]) {

    /* Asociamos el socket a un puerto local */
    struct sockaddr_in server_addr;
    socklen_t server_size = sizeof(server_addr);
    memset(&server_addr, 0, server_size);
    server_addr.sin_family = AF_INET; // tipo de conexión (por red o interna)
    server_addr.sin_port = htons(SERVER_PORT); // es el número que le corresponde al puerto

    pthread_t send_thread, recv_thread;

    //CREACION DEL SOCKET

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        error_socket();
    }
    //connect solicita la conexión con el servidor. 0 --> establece la conexión si no se ejecuta un error
    if (connect(sock, (struct sockaddr*)&server_addr, server_size) < 0) {
        error_connect_server();
    } else {
        if (pthread_create(&send_thread, NULL, send_msg, (void*)&sock) < 0) {
            error_send_thread_create();
        }

        if (pthread_create(&recv_thread, NULL, recv_msg, (void*)&sock) < 0) {
            error_recv_thread_create();
        }

        pthread_join(send_thread, NULL);
        pthread_join(recv_thread, NULL);
    }
    close(sock);
    return 0;
}