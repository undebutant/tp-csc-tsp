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
    char nomFic [5][80];
    int debut;
    int fin;
    int libre;
    int occupe;
};

pthread_mutex_t mutex;
pthread_mutex_t mutexpthread;

void *impression(void * buffer){
  char Jacky [80];
  struct hack * main = (struct hack *)buffer;

  while(1) {

	  int rp;
	    rp = pthread_mutex_lock(&mutexpthread);
	      if(rp)
		error(EXIT_FAILURE, rp, "pthread_mutex_lock");

	  while(main->occupe==0) {};
	  int rc;
	    rc = pthread_mutex_lock(&mutex);
	      if(rc)
		error(EXIT_FAILURE, rc, "mutex_lock");

	  
	  
	  strcpy(Jacky,main->nomFic[main->debut]);
	  main->debut=(main->debut + 1)%5;
	   
	  main->libre++;
	  main->occupe--;
	  


	  printf("thread : libre : %d, occupe : %d, debut : %d, fin : %d\n",main->libre,main->occupe,main->debut,main->fin);

	  rc = pthread_mutex_unlock(&mutex);
	      if(rc)
		error(EXIT_FAILURE, rc, "mutex_unlock");

	  
	  rp = pthread_mutex_unlock(&mutexpthread);
	      if(rp)
		error(EXIT_FAILURE, rp, "pthread_mutex_unlock");



	  printf("Debut d'impression de \"%s\"...\n", Jacky);
	  sleep(5); /* Simule la lenteur de l'impression */
	  printf("...Fin d'impression de \"%s\"...\n", Jacky);
  
  }
  
  pthread_exit(0);
}

void attenteImpression(){
  struct hack * buffer = malloc(sizeof(struct hack));
  buffer->debut=0;
  buffer->fin=0;
  buffer->libre=5;
  buffer->occupe=0;

  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&mutexpthread, NULL);

  pthread_t thread1,thread2,thread3;
  pthread_create(&thread1,NULL,impression,(void *)buffer);
  pthread_create(&thread2,NULL,impression,(void *)buffer);
  pthread_create(&thread3,NULL,impression,(void *)buffer);
  char nomFic[80];
   
  do {
    
    
    printf("Nom du fichier a imprimer (taper '0' pour terminer) ? ");
    scanf("%s", nomFic);
    if (strcmp(nomFic,"0") == 0){
      break;
    }

    while(buffer->libre==0);
    
    int rc;
    rc = pthread_mutex_lock(&mutex);
      if(rc)
	error(EXIT_FAILURE, rc, "mutex_lock");

    strcpy(buffer->nomFic[buffer->fin],nomFic);
    buffer->fin=(buffer->fin + 1)%5;
   
    buffer->libre--;
    buffer->occupe++;
    printf("main : libre : %d, occupe : %d, debut : %d, fin : %d\n",buffer->libre,buffer->occupe,buffer->debut,buffer->fin);
    rc = pthread_mutex_unlock(&mutex);
      if(rc)
	error(EXIT_FAILURE, rc, "mutex_unlock");

    
    
    
    
  }while (1);
  printf("Au revoir\n");
  free(buffer);
}

int main(){

  attenteImpression();
  return EXIT_SUCCESS;

} char Jacky [80];
