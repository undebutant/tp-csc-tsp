/*
 * Programme simulant un serveur de requêtes
 */

/* Correspond au corrigé de l'exercice 3.2 du TP noté de 2009 */

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

/**
   Fonction envoyant la requete @a req vers l'un des threads traiteurs
   via le medium de communication
   @param req (donnee) Requete a envoyer
*/
void envoyerVersThreadTraiteur(char *req)
{
  if (write(distr_traite[1], &req, sizeof(req)) < sizeof(req)){
    perror("write");
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

  /* "Reception" de requetes client et envoi vers les threads traiteur */
  for (i = nbReq ; i > 0 ; i--){
    /* Allocation de la zone pour stocker la requete */
    char *req = malloc(TAILLE_MAX_REQUETE);
    if (req == NULL){
      error(EXIT_FAILURE, 0, "malloc");
    }

    /* Recuperation d'une requete client */
    simulerReceptionRequeteDeClient(req);

    /* Distribution de la requete a l'un des threads traiteur */
    envoyerVersThreadTraiteur(req);
  }

  /* Envoi du message de terminaison a tous les threads traiteur */
  for (i = N ; i > 0 ; i--){
    /* Allocation de la zone pour stocker la requete */
    char *req = malloc(sizeof('\0'));
    if (req == NULL){
      error(EXIT_FAILURE, 0, "malloc");
    }

    /* Initialisation de la zone */
    req[0] = '\0';

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
  char *req;
  int numThread = (int)args;

  while (1){
    /* Attente/Reception d'une requete du thread distributeur */
    if (read(distr_traite[0], &req, sizeof(req)) < sizeof(req)){
      perror("read");
      exit(EXIT_FAILURE);
    }
      
    /* On verifie que le thread distributeur ne nous demande pas de
       nous arreter.
    */
    if (req[0] == '\0'){
      free(req);
      req = NULL;
      break;
    }

    /* Traitement de la requete */
    simulerTraitementEtEnvoiReponseAuClient(req,numThread);

    /* On libere cette zone memoire */
    free(req);
    req = NULL;
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

