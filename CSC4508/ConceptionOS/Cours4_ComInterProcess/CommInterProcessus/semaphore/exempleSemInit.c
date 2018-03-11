/********************/
/* exempleSemInit.c */
/********************/

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#include "exempleSem.h"
#define NB_PLACES 2

int main(int argc, char *argv[]){
  sem_t *semParking;

  if (argc != 1) {
    fputs("USAGE = exempleSemInit\n", stderr);
    exit(EXIT_FAILURE);
  }

  /* Création et initialisation du sémaphore */
  semParking = sem_open(CLE_PARKING, O_CREAT, S_IRWXU, NB_PLACES);
  if (semParking == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }
  puts("Initialisation OK, vous pouvez executer exempleSem");

  int sval = -1;
  if(sem_getvalue(semParking, &sval) <0) {
    perror("sem_getvalue");
    exit(EXIT_FAILURE);
  }
  printf("sval = %d\n", sval);

  return EXIT_SUCCESS;
}
