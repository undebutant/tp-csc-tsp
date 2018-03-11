/*********************************/
/* serveurTCP_1_tache_n_client.c */
/*********************************/

#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h> // To get TCP_NODELAY declaration
#include <netdb.h>
#include <unistd.h>

#include "initCommunication.h"
#include "client-serveur.h"

#include <event2/event.h>
struct event_base *base;

//
// Constantes de fonctionnement du programme
//
//
#define NB_MAX_REPONSES 10 // Nombre maximum de reponses que le serveur fait
                           // a un client (avant de lui repondre REPONSE_KO) 

//
// Structure pour la callback
typedef struct{
  int nbReponsesAuClient;
  struct event *ev;
} contexte_t;

void gestionConnexion(int fdConnexion, short event, void *arg){
  // On cree un contexte pour gagner en efficacite
  contexte_t *pctxt = (contexte_t *)arg;
  message_t requete;

  ssize_t nbRead = read(fdConnexion, &requete, sizeof(requete));
  if (nbRead == sizeof(requete)) {
    message_t reponse;
    /* On traite la requete */
    if (pctxt->nbReponsesAuClient < NB_MAX_REPONSES) {
      pctxt->nbReponsesAuClient++;
      reponse.typ = REQUETE_OK;
      CALCUL_PAYLOAD(reponse.infoSup.payload, requete.infoSup.idClient, pctxt->nbReponsesAuClient);
      //usleep(100000);
    } else {
      reponse.typ = REQUETE_KO;
    }
    ssize_t nbWrite = write(fdConnexion, &reponse, sizeof(reponse));
    if (nbWrite < sizeof(reponse))
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "Pb ecriture sur tube nomme");
    // On reamorce l'evenement
    event_add(pctxt->ev, NULL);
  } else {
    /* Le client a ferme la connexion : on fait le menage */
    if (close(fdConnexion) < 0)
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "close");
    free(pctxt);
  }
}

void gestionPointDAcces(int fdPointDAcces, short event, void *arg) {
  static int rangClient = 0;

  int fdConnexion = accept(fdPointDAcces, NULL, NULL);
  if (fdConnexion < 0)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "accept");

  // We set TCP_NODELAY flag so that packets sent on this TCP connection
  // will not be delayed by the system layer
  int status=1;
  if (setsockopt(fdConnexion, IPPROTO_TCP, TCP_NODELAY, &status, sizeof(status)) < 0)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "setsockopt");

  // Gestion de cette connexion
  rangClient++;
  printf("%6d-ieme client a traiter\n", rangClient);

  contexte_t *pctxt = malloc(sizeof(contexte_t));
  if (pctxt == NULL)
    error_at_line(EXIT_FAILURE, 0, __FILE__, __LINE__, "malloc");
  //
  // Initialisation du contexte
  //
  pctxt->nbReponsesAuClient = 0;
  // Definition de l'evenement 
  pctxt->ev = event_new(base, fdConnexion, EV_READ, gestionConnexion, pctxt);

  /* Ajout aux evenements actifs, sans timeout */
  event_add(pctxt->ev, NULL);
}

int main() {
  int fdPointDAcces = creationPointDAcces();

  // Initialisation de la librairie event
  base = event_base_new();
  if (!base) {
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "Could not initialize libevent!\n");
  }

  // Definition de l'evenement lie au point d'acces
  struct event *eventPointDAcces = event_new(base, fdPointDAcces, EV_READ|EV_PERSIST, gestionPointDAcces, NULL);

  // Ajout aux evenements actifs, sans timeout
  event_add(eventPointDAcces, NULL);

  //
  // Attente d'evenements
  //
  event_base_dispatch(base);

  return EXIT_SUCCESS;
}
