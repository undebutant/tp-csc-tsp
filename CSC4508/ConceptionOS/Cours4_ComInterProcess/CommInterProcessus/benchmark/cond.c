/*
 * Programme simulant un serveur de requêtes
 */

/* Correspond au corrigé de l'exercice 3.4 du TP noté de 2009 */

#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <error.h>
#include <time.h>
#include "simulateur.h"

#define TAILLE_MAX_REQUETE 4096

#define N 10

#define TAILLE_TAB (2*N)
char tab_distr_traite[TAILLE_TAB][TAILLE_MAX_REQUETE];

int placeDispo = TAILLE_TAB;
int infoPrete = 0;

int iDepot = 0;
int iExtrait = 0;

pthread_mutex_t mutex;

pthread_cond_t condPlaceDispo;
pthread_cond_t condInfoPrete;


/**
   Fonction envoyant la requete @a req vers l'un des threads traiteurs
   via le medium de communication
   @param req (donnee) Requete a envoyer
*/
void envoyerVersThreadTraiteur(char *req)
{
  int rc;
  rc = pthread_mutex_lock(&mutex);
  if (rc != 0){
    error(EXIT_FAILURE, rc, "pthread_mutex_lock");
  }
  infoPrete++;
  pthread_cond_signal(&condInfoPrete);
  rc = pthread_mutex_unlock(&mutex);
  if (rc != 0){
    error(EXIT_FAILURE, rc, "pthread_mutex_unlock");
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
    rc = pthread_mutex_lock(&mutex);
    if (rc != 0){
	error(EXIT_FAILURE, rc, "pthread_mutex_lock");
    }
    while (placeDispo == 0){
      pthread_cond_wait(&condPlaceDispo, &mutex);
    }
    req = tab_distr_traite[iDepot];
    iDepot = (iDepot +1) % TAILLE_TAB;
    placeDispo--;
    rc = pthread_mutex_unlock(&mutex);
    if (rc != 0){
      error(EXIT_FAILURE, rc, "pthread_mutex_unlock");
    }

    /* Recuperation d'une requete client */
    simulerReceptionRequeteDeClient(req);

    /* Distribution de la requete a l'un des threads traiteur */
    envoyerVersThreadTraiteur(req);
  }

  /* Envoi du message de terminaison a tous les threads traiteur */
  for (i = N ; i > 0 ; i--){
    /* "Allocation" d'une requête */
    rc = pthread_mutex_lock(&mutex);
    if (rc != 0){
	error(EXIT_FAILURE, rc, "pthread_mutex_lock");
    }
    while (placeDispo == 0){
      pthread_cond_wait(&condPlaceDispo, &mutex);
    }
    tab_distr_traite[iDepot][0] = '\0';
    iDepot = (iDepot +1) % TAILLE_TAB;
    placeDispo--;
    rc = pthread_mutex_unlock(&mutex);
    if (rc != 0){
      error(EXIT_FAILURE, rc, "pthread_mutex_unlock");
    }

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
      rc = pthread_mutex_lock(&mutex);
      if (rc != 0){
	error(EXIT_FAILURE, rc, "pthread_mutex_lock");
      }
      while (infoPrete == 0){
	pthread_cond_wait(&condInfoPrete, &mutex);
      }
      strncpy(req, tab_distr_traite[iExtrait], TAILLE_MAX_REQUETE);
      iExtrait = (iExtrait +1) % TAILLE_TAB;
      infoPrete--;
      placeDispo++;
      pthread_cond_signal(&condPlaceDispo);
      rc = pthread_mutex_unlock(&mutex);
      if (rc != 0){
	error(EXIT_FAILURE, rc, "pthread_mutex_unlock");
      }
      
      /* On verifie que le thread distributeur ne nous demande pas de
	 nous arreter.
      */
      if (req[0] == '\0'){
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
  rc = pthread_mutex_init(&mutex, NULL);
  if (rc != 0){
    error(EXIT_FAILURE, rc, "pthread_mutex_init");
  }

  rc = pthread_cond_init(&condPlaceDispo, NULL);
  if (rc != 0){
    error(EXIT_FAILURE, rc, "pthread_cond_init");
  }

  rc = pthread_cond_init(&condInfoPrete, NULL);
  if (rc != 0){
    error(EXIT_FAILURE, rc, "pthread_cond_init2");
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

  /* That's all, folks ! */
  return EXIT_SUCCESS;
}

