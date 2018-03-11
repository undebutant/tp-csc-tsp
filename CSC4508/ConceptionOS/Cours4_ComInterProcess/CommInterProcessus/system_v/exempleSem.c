/****************/
/* exempleSem.c */
/****************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#include "exempleSem.h"

void P(int semId){
  struct sembuf op;
  op.sem_num =  0;
  op.sem_op  = -1;
  op.sem_flg =  0;
  if (semop(semId, &op, 1) == -1){
    perror("semop");
    exit(EXIT_FAILURE);
  }
}

void V(int semId){
  struct sembuf op = {0, 1, 0};
  if (semop(semId, &op, 1) == -1){
    perror("semop");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]){
  int semParking;
  
  if (argc != 1) {
    fputs("USAGE = exempleSem\n", stderr);
    exit(EXIT_FAILURE);
  }
  semParking = semget(CLE_PARKING, 1, 0);
  if (semParking == -1) {
    perror("semget");
    exit(EXIT_FAILURE);
  }
  printf("Demande d'entree dans le parking...\n");
  P(semParking);
  printf("\a...OK\n");
  sleep(5);
  printf("Avertir de sortie du parking...\n");
  V(semParking);
  printf("\a...OK\n");
  sleep(5);
  return EXIT_SUCCESS;
}
