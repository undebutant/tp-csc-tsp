/**
   @file simulateur.c

   Module contenant toutes les operations de simulation (pour eviter d'avoir de vrais client face au serveur)
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sched.h>
#include "simulateur.h"

/**
   Variable indiquant s'il faut faire des printf au niveau de @a
   simulerTraitementEtEnvoiReponseAuClient ou non
*/
int printfAFaire = 0;

/**
   Nombre max de requetes au dela duquel on ne fait plus de printf
*/
#define MAX_REQ_POUR_PRINTF 100

/**
   Initialisation du simulateur
   @param nbReq (donnee) Nombre de requetes que devra traiter le simulateur
*/
void initSimulateur(int nbReq)
{
  /* On initialise le generateur de nombre aleatoire toujours de la
     meme maniere de sorte que deux invocations au serveur donnent
     exactement le meme comportement au niveau
  */
  srand(0);
  /* On ne fait des printf au niveau de simulerReceptionRequeteDeClient que
     si nbReq est inferieur a un certain seuil (ainsi, les comparaison de 
     performances ne sont pas polluees par des appels a printf qui cacheraient
     des choses)
  */
  printfAFaire = (nbReq < MAX_REQ_POUR_PRINTF);
}

/**
   Simulation de la reception d'une requete envoyee par un client au serveur.
   @param req (donnee-resultat) Zone ou doit etre stockee la requete que le 
   serveur est suppose avoir recu du client.
*/
void simulerReceptionRequeteDeClient(char *req)
{
  int pourcentage = rand() % 100;

  /* Calcul de la longueur de la requete */
  int lngReq;
  if (pourcentage < 85){
    lngReq = 128;
  } else if (pourcentage < 95) {
    lngReq = 1024;
  }else {
    lngReq = 4095;
  }

  /* Calcul de la requete */
  memset(req, 'a', lngReq);
  req[lngReq] = '\0';
}

/**
   Simulation du traitement de la requete @a req et de son envoi vers le client
   @param req (donnee) Zone ou est stockee la requete a traiter
   @param nbThread (donnee) Numero du thread qui a invoque cette operation (est affiche dans le printf).
*/
void simulerTraitementEtEnvoiReponseAuClient(char *req, int numThread)
{
  if (printfAFaire){
    printf("Thread %2d : Traitement et envoi d'une requete de %4d octets\n",
	   numThread,
	   strlen(req));
  }
  /* On lache la main pour permettre a un autre thread de prendre la main
     (sinon dans certaines situations, c'est toujours le meme thread qui
     s'execute).
  */
  if (sched_yield() < 0){
    perror("sched_yield");
    exit(EXIT_FAILURE);
  }
}
  


