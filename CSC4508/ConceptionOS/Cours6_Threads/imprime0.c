/* 
 * Programme simulant la gestion d'une imprimante
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <error.h>

struct hack {
    char nomFic [80];
    int att;
};

pthread_mutex_t mutex;

void *impression(void * buffer){
  struct hack * main = (struct hack *)buffer;
  char Jacky [80];
  strcpy(Jacky,main->nomFic);
  main->att=1;
  int rc;
  rc = pthread_mutex_lock(&mutex);
      if(rc)
	error(EXIT_FAILURE, rc, "pthread_mutex_lock");

  printf("Debut d'impression de \"%s\"...\n", Jacky);
  sleep(5); /* Simule la lenteur de l'impression */
  printf("...Fin d'impression de \"%s\"...\n", Jacky);


  rc = pthread_mutex_unlock(&mutex);
      if(rc)
	error(EXIT_FAILURE, rc, "pthread_mutex_unlock");

  pthread_exit(0);
}

void attenteImpression(){
  struct hack * buffer = malloc(sizeof(struct hack));
  buffer->att=0;
  pthread_mutex_init(&mutex, NULL); 
  do {
    buffer->att=0;
    printf("Nom du fichier a imprimer (taper '0' pour terminer) ? ");
    scanf("%s", buffer->nomFic);
    if (strcmp(buffer->nomFic,"0") == 0){
      break;
    }
    pthread_t monthread;
    pthread_create(&monthread,NULL,impression,(void *)buffer);
    while(buffer->att==0);
    
    
  }while (1);
  printf("Au revoir\n");
  free(buffer);
}

int main(){

  attenteImpression();
  return EXIT_SUCCESS;

}
