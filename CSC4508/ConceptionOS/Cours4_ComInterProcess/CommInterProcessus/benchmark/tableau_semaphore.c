/*
 * Programme simulant un serveur de requêtes
 */

/* Correspond au corrigé de l'exercice 3.3 du TP noté de 2009 */

#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <error.h>
#include <time.h>
#include <semaphore.h>

#include "simulateur.h"

#define TAILLE_MAX_REQUETE 4096

#define N 10

#define TAILLE_TAB (2*N)
char tab_distr_traite[TAILLE_TAB][TAILLE_MAX_REQUETE];

sem_t placeDispo;
sem_t infoPrete;
pthread_mutex_t mutex;

int iDepot = 0;
int iExtrait = 0;


/**
   Fonction envoyant la requete @a req vers l'un des threads traiteurs
   via le medium de communication
   @param req (donnee) Requete a envoyer
*/
void envoyerVersThreadTraiteur(char *req)
{
  int rc;
  rc = sem_post(&infoPrete);
  if (rc) {
    error(EXIT_FAILURE, rc, "sem_post");
  }
}

/**
   Code du thread distributeur
   @param nbReq (donnee) Nombre de requetes a envoyer vers les threads traiteurs
*/
void distributeur(int nbReq)
{
  int i;
  char *req;
  int rc;

  /* "Reception" de requetes client et envoi vers les threads traiteur */
  for (i = nbReq ; i > 0 ; i--){
    /* "Allocation" d'une requête */
    rc = sem_wait(&placeDispo);
    if (rc){
      error(EXIT_FAILURE, rc, "sem_wait");
    }
    req = tab_distr_traite[iDepot];
    iDepot = (iDepot +1) % TAILLE_TAB;

    /* Recuperation d'une requete client */
    simulerReceptionRequeteDeClient(req);

    /* Distribution de la requete a l'un des threads traiteur */
    envoyerVersThreadTraiteur(req);
  }

  /* Envoi du message de terminaison a tous les threads traiteur */
  for (i = N ; i > 0 ; i--){
    /* "Allocation" d'une requête */
    rc = sem_wait(&placeDispo);
    if (rc){
      error(EXIT_FAILURE, rc, "sem_wait");
    }
    tab_distr_traite[iDepot][0] = '\0';
    iDepot = (iDepot +1) % TAILLE_TAB;

    /* Distribution du message a l'un des threads traiteur */
    envoyerVersThreadTraiteur(req);
  }
}

/**
   Code du thread traiteur
   @param args (donnees) Contient le numero du thread
*/
void *traiteur(void *args) 
{
  char req[TAILLE_MAX_REQUETE];
  int numThread = (int)args;
  int rc;

  while (1)
    {
      /* Attente/Reception d'une requete du thread distributeur */
      rc = sem_wait(&infoPrete);
      if (rc){
	error(EXIT_FAILURE, rc, "sem_wait");
      }
      rc = pthread_mutex_lock(&mutex);
      if (rc != 0){
	error(EXIT_FAILURE, rc, "pthread_mutex_lock");
      }
      strncpy(req, tab_distr_traite[iExtrait], TAILLE_MAX_REQUETE);
      iExtrait = (iExtrait +1) % TAILLE_TAB;
      rc = pthread_mutex_unlock(&mutex);
      if (rc != 0){
	error(EXIT_FAILURE, rc, "pthread_mutex_unlock");
      }
      rc = sem_post(&placeDispo);
      if (rc) {
	error(EXIT_FAILURE, rc, "sem_post");
      }
      
      /* On verifie que le thread distributeur ne nous demande pas de
	 nous arreter.
      */
      if (req[0] == '\0'){
	rc = sem_post(&placeDispo);
	if (rc) {
	  error(EXIT_FAILURE, rc, "sem_post");
	}
	break;
      }

      /* Traitement de la requete */
      simulerTraitementEtEnvoiReponseAuClient(req,numThread);
    }

  pthread_exit(NULL);
}

/**
   Fonction principale. Realise toutes les initialisations
*/
int main(int argc, char *argv[]){
  int i;
  pthread_t threadPool[N];
  int nbReq;
  int rc;

  if (argc != 2){
    fprintf(stderr, "USAGE = serveur nbRequetesASimuler\n");
    exit(EXIT_FAILURE);
  }

  nbReq = atoi(argv[1]);

  /* Initialisation du module simulateur */
  initSimulateur(nbReq);

  /* Creation du medium de communication */
  rc = sem_init(&placeDispo, 0, TAILLE_TAB);
  if (rc != 0){
    error(EXIT_FAILURE, rc, "sem_init");
  }

  rc = sem_init(&infoPrete, 0, 0);
  if (rc != 0){
    error(EXIT_FAILURE, rc, "sem_init");
  }

  rc = pthread_mutex_init(&mutex, NULL);
  if (rc != 0){
    error(EXIT_FAILURE, rc, "pthread_mutex_init");
  }

  struct timespec t1, t2;
  clock_gettime(CLOCK_MONOTONIC, &t1);

  /* Fork des threads traiteur */
  for (i = 0; i < N ; i++){
    errno = pthread_create(&(threadPool[i]), NULL, traiteur, (void *)i);
    if (errno != 0){
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  /* Demarrage du distributeur */
  distributeur(nbReq);

  /* Attente de la terminaison de tous les threads traiteurs */
  for (i = 0; i < N; i ++){
    errno = pthread_join(threadPool[i], NULL);
    if (errno != 0){
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &t2);
  double duration = (t2.tv_sec - t1.tv_sec)*1e9 + (t2.tv_nsec - t1.tv_nsec);
  printf("%d requests in %lf ms (%f usec per request)\n", nbReq,
	 duration/1e6, (duration/1e3)/nbReq);

  /* Liberation du medium de communication */
  rc = sem_destroy(&placeDispo);
  if (rc != 0){
    error(EXIT_FAILURE, rc, "sem_init");
  }

  rc = sem_destroy(&infoPrete);
  if (rc != 0){
    error(EXIT_FAILURE, rc, "sem_init");
  }

  rc = pthread_mutex_destroy(&mutex);
  if (rc != 0){
    error(EXIT_FAILURE, rc, "pthread_mutex_destroy");
  }

  /* That's all, folks ! */
  return EXIT_SUCCESS;
}

