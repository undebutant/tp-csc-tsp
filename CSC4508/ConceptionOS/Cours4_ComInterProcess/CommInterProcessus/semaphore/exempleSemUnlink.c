/********************/
/* exempleSemUnlink.c */
/********************/

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#include "exempleSem.h"

int main(int argc, char *argv[]){
  if (argc != 1) {
    fputs("USAGE = exempleSemUnlink\n", stderr);
    exit(EXIT_FAILURE);
  }

  /* delete the named semaphore */
  if( sem_unlink(CLE_PARKING) < 0) {
    perror("sem_unlink");
    exit(EXIT_FAILURE);
  }
  return EXIT_SUCCESS;
}
