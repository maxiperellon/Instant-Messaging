#include "header.h"

int main(int argc, char *argv[]) {

    /* Asociamos el socket a un puerto local */
    struct sockaddr_in server_addr;
    socklen_t server_size = sizeof(server_addr);
    memset(&server_addr, 0, server_size);
    server_addr.sin_family = AF_INET; //tipo de conexión (por red o interna)
    server_addr.sin_port = htons(SERVER_PORT); //es el numero que le corresponde al puerto

    //CREACION DEL SOCKET

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0){
        printf("Error al crear el socket...\n");
        exit(0);
    }
    //connect solicita la conexion con el servidor. 0 --> establece la conexion si no larga error
    if (connect(sock, (struct sockaddr*)&server_addr, server_size) < 0) {
        printf("could not connect to server\n");
        return 1;
    }
    printf("\n\t\t\t>>> BIENVENIDO AL CHAT <<<<\n\n");
}