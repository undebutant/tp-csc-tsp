/********************/
/* exempleSemInit.c */
/********************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "exempleSem.h"
#define NB_PLACES 2

int main(int argc, char *argv[]){
  int semParking;
  
  if (argc != 1) {
    fputs("USAGE = exempleSemInit\n", stderr);
    exit(EXIT_FAILURE);
  }
  /* Création du sémaphore */
  semParking = semget(CLE_PARKING, 1, IPC_CREAT|0666);
  if (semParking == -1) {
    perror("semget");
    exit(EXIT_FAILURE);
  }
  /* Intialisation du sémaphore au nombre de places dans le parking */
  if (semctl(semParking, 0, SETVAL, NB_PLACES) == -1) {
    perror("semctl");
    exit(EXIT_FAILURE);
  }
  puts("Initialisation OK, vous pouvez executer exempleSem");
  return EXIT_SUCCESS;
}
