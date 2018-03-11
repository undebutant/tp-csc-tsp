/****************/
/* exempleSem.c */
/****************/

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <unistd.h>

#include "exempleSem.h"

void P(sem_t *sem_id){
  sem_wait(sem_id);
}

void V(sem_t *sem_id){
  sem_post(sem_id);
}

int main(int argc, char *argv[]){
  sem_t *semParking;

  if (argc != 1) {
    fputs("USAGE = exempleSem\n", stderr);
    exit(EXIT_FAILURE);
  }

  /* ouverture du sémaphore deja créé */
  semParking = sem_open(CLE_PARKING, 0);
  if (semParking == SEM_FAILED) {
    perror("sem_open");
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
