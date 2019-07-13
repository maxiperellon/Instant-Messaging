#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <errno.h>

#include <stdio.h>
extern int errno;
#define SEMPERM 0600
#define TRUE  1
#define FALSE 0


int sem_verde(int semid);
int initsem(key_t semkey);

int sem_rojo(int semid);
void setSemid(int id);
int getSemid();

