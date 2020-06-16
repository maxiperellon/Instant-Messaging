//
// Created by Maximiliano Perellón on 31/01/2020.
//
#include "header.h"

void* recv_msg(void* r) {
    int* id;
    char buffer[MAXLINE];
    id = (int*) r;

    while(1) {
        recv(*id, buffer, MAXLINE, 0);
        printf("%s\n", buffer);
        fflush(stdout);
        sleep(3);
    }
}

void* send_msg(void* s) {
    int* id;
    char buffer[MAXLINE];
    id = (int*) s;

    printf("\n\t\t\t>>> BIENVENIDO AL CHAT <<<<\n\n");
    printf("\tIntroduce ADD y tu nombre de usuario \n");
    printf("\tADD: 'Agrega tu nombre de usuario al servidor' \n");


    while(1) {
        printf("\t\n-> ");
        fgets(buffer, MAXLINE, stdin);
        strtok(buffer,"\n");
        send(*id, buffer, MAXLINE, 0);
        sleep(3);
    }
}
