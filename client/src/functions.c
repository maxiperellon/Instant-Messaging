#include "header.h"

void* recv_msg(void* r) {
    int* id;
    char buffer[MAX_LINE_LEN];
    id = (int*) r;

    while(1) {
        recv(*id, buffer, MAX_LINE_LEN, 0);
        printf("%s\n", buffer);
        fflush(stdout);
        sleep(3);
    }
}

void* send_msg(void* s) {
    int* id;
    char buffer[MAX_LINE_LEN];
    id = (int*) s;

    system("clear");
    printf("\n\t\t\t>>> BIENVENIDO AL CHAT <<<<\n\n");
    printf("\tIntroduce ADD y tu nombre de usuario \n");
    printf("\tADD: 'Agrega tu nombre de usuario al servidor' \n");

    while(1) {
        printf("\t\n-> ");
        fgets(buffer, MAX_LINE_LEN, stdin);
        strtok(buffer,"\n");
        send(*id, buffer, MAX_LINE_LEN, 0);
        sleep(1);
    }
}
