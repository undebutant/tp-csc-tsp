/* 
 * Programme simulant la gestion d'une imprimante
 * Dans cette version, on a un processus client (imprime4c) qui envoie
 * ses requetes d'impression a un processus serveur (imprime4s) via un
 * tube nommé
 */


#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>
#include <semaphore.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include "ex5.h"

struct shared_memory *buffer;

void attenteImpression(){
  char nomFic [TAILLEMAX];
  printf("Demarrage du client\n");
  int fd;
  fd = shm_open("lapin",O_RDWR,0666);
  if(fd<0) {
	printf("Can't create the shared memory\n");
	abort();
  }
  
  int i = ftruncate(fd,SHM_SIZE);
  assert(i>=0);
  if((buffer = mmap(NULL, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) <0) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  
    printf("Preparation terminée\n");
  /* C'est parti pour les impressions */
  do {
    sem_wait(&buffer->place_libre);
    printf("Nom du fichier a imprimer (taper '0' pour terminer) ? \n");
    scanf("%s", nomFic);
    if (strcmp(nomFic,"0") == 0){
      sem_post(&buffer->place_libre);
      break;
    }
    printf("Fichier envoyé : '%s'\n",nomFic);
    strncpy(buffer->tab[buffer->finecrit],nomFic,TAILLEMAX);
    
    buffer->finecrit= (buffer->finecrit + 1)%CASEMAX;
    sem_post(&buffer->info_occupe);
  }while (1);
    
  printf("Au revoir\n");

}

int main(){

  attenteImpression();
  return EXIT_SUCCESS;

}
