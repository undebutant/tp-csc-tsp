/* 
 * Programme simulant la gestion d'une imprimante
 * Dans cette version, on a un processus client (imprime4c) qui envoie
 * ses requetes d'impression a un processus serveur (imprime4s) via un
 * tube nommé
 */


//////Les semaphores et shm dans /dev/shm

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
#include "ex5.h"



struct shared_memory *buffer;

void impression(char *nomFic){

  printf("Debut d'impression de \"%s\"...\n", nomFic);
  sleep(5); /* Simule la lenteur de l'impression */
  printf("...Fin d'impression de \"%s\"...\n", nomFic);

}

int main(){
  printf("Demarrage du serveur charge des impressions\n");
  int fd;
  fd = shm_open("lapin",O_CREAT|O_RDWR,0666);
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
  
  buffer->debutlect=0;
  buffer->finecrit=0;
  sem_init(&buffer->info_occupe,1,0);
  sem_init(&buffer->place_libre,1,CASEMAX);
  
  
  
  printf("Preparation terminée\n");
  
  /* Attente des requêtes */
  do{
    sem_wait(&buffer->info_occupe);
    
    impression(buffer->tab[buffer->debutlect]);

    buffer->debutlect= (buffer->debutlect + 1)%CASEMAX;
    sem_post(&buffer->place_libre);
  
  }while(1);

}
