/* 
 * Programme simulant la gestion d'une imprimante
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>
#include <assert.h>

sem_t* mutex = NULL;

void impression(char *nomFic){

  printf("Debut d'impression de \"%s\"...\n", nomFic);
  sleep(5); /* Simule la lenteur de l'impression */
  printf("...Fin d'impression de \"%s\"...\n", nomFic);

}

void attenteImpression(){
  char nomFic[80];
  
  mutex = sem_open("/pingouin", O_CREAT, S_IRWXU, 1);
  if(mutex== SEM_FAILED) {
    fprintf(stderr, "sem_open failed: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  // On peut le unlink direct, on ne compte pas faire d'autre open dessus
  if (sem_unlink("/pingouin") == -1) {
    fprintf(stderr, "sem_unlink failed: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  
  do {
    printf("Nom du fichier a imprimer (taper '0' pour terminer) ? ");
    scanf("%s", nomFic);
    if (strcmp(nomFic,"0") == 0){
      break;
    }

      int pid = fork();
      if(pid == -1) {
	perror("\nFork failed, ");
	fprintf(stderr, "error returned: %s\n", strerror(errno));
      }
      else if(!pid) {
	printf("Vérification du statut de l'imprimante");
	int i = sem_wait(mutex);
	assert(i==0);
	impression(nomFic);
	i = sem_post(mutex);
	assert(i==0);
	return;
      }

  }while (1);
  printf("Au revoir\n");
}

int main(){
  attenteImpression();
  return EXIT_SUCCESS;
}
