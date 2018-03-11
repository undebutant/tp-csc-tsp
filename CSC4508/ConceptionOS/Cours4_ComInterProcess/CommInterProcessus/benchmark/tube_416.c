/*
 * Programme simulant un serveur de requêtes
 */

/* Correspond au corrigé de l'exercice 3.1 du TP noté de 2009 */

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

int distr_traite[2];

pthread_mutex_t mutex;

/**
   Fonction envoyant la requete @a req vers l'un des threads traiteurs
   via le medium de communication
   @param req (donnee) Requete a envoyer
*/
void envoyerVersThreadTraiteur(char *req)
{
  int lngReq = strlen(req)+sizeof('\0');
  if (write(distr_traite[1], &lngReq, sizeof(lngReq)) < sizeof(lngReq)){
    perror("write");
    exit(EXIT_FAILURE);
  }
  if (write(distr_traite[1], req, lngReq) < lngReq){
    perror("write2");
    exit(EXIT_FAILURE);
  }
}

/**
   Code du thread distributeur
   @param nbReq (donnee) Nombre de requetes a envoyer vers les threads traiteurs
*/
void distributeur(int nbReq)
{
  int i;
  char req[TAILLE_MAX_REQUETE];

  /* "Reception" de requetes client et envoi vers les threads traiteur */
  for (i = nbReq ; i > 0 ; i--){
    /* Recuperation d'une requete client */
    simulerReceptionRequeteDeClient(req);

    /* Distribution de la requete a l'un des threads traiteur */
    envoyerVersThreadTraiteur(req);
  }

  /* Envoi du message de terminaison a tous les threads traiteur */
  req[0] = '\0';
  for (i = N ; i > 0 ; i--){
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

  while (1)
    {
      int lngReq;
      int rc;
      /* Attente/Reception d'une requete du thread distributeur.
	 On entre en eclusion mutuelle pour etre sur que le thread qui fait
	 le premier read ne se fera pas doubler par un autre thread au
	 momemnt du 2eme read.
      */
      rc = pthread_mutex_lock(&mutex);
      if(rc){
	error(EXIT_FAILURE, rc, "pthread_mutex_lock");
      }

      if (read(distr_traite[0], &lngReq, sizeof(lngReq)) < sizeof(lngReq)){
	perror("read");
	exit(EXIT_FAILURE);
      }
      if (read(distr_traite[0], req, lngReq) < lngReq){
	perror("read2");
	exit(EXIT_FAILURE);
      }

      rc = pthread_mutex_unlock(&mutex);
      if(rc){
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
  if (pipe(distr_traite) < 0){
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  /* Creation du mutex utilise par les threads traiteurs */
  rc = pthread_mutex_init(&mutex, NULL);
  if(rc){
    error(EXIT_FAILURE, rc, "pthread_mutex_lock");
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
  if (close(distr_traite[0]) < 0){
    perror("close");
    exit(EXIT_FAILURE);
  }

  if (close(distr_traite[1]) < 0){
    perror("close2");
    exit(EXIT_FAILURE);
  }

  /* That's all, folks ! */
  return EXIT_SUCCESS;
}

