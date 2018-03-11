/*********************************/
/* serveurTCP_1_tache_1_client.c */
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

//
// Constantes de fonctionnement du programme
//
//
#define NB_MAX_REPONSES 10 // Nombre maximum de reponses que le serveur fait
                           // a un client (avant de lui repondre REPONSE_KO) 

void gestionConnexion(int fdConnexion){
  ssize_t nbRead;
  message_t requete;
  int nbReponsesAuClient = 0;

  while ((nbRead = read(fdConnexion, &requete, sizeof(requete))) == sizeof(requete)) {
    message_t reponse;
    /* On traite la requete */
    if (nbReponsesAuClient < NB_MAX_REPONSES) {
      nbReponsesAuClient++;
      reponse.typ = REQUETE_OK;
      CALCUL_PAYLOAD(reponse.infoSup.payload, requete.infoSup.idClient, nbReponsesAuClient);
      //usleep(100000);
    } else {
      reponse.typ = REQUETE_KO;
    }
    ssize_t nbWrite = write(fdConnexion, &reponse, sizeof(reponse));
    if (nbWrite < sizeof(reponse))
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "Pb ecriture sur tube nomme");
  }
  /* Le client a ferme la connexion : on fait le menage */
  if (close(fdConnexion) < 0)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "close");
}

void gestionPointDAcces(int fdPointDAcces) {
  int rangClient = 0;
  do {
    // Attente d'une connexion
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

    gestionConnexion(fdConnexion);
  } while (1);
}

int main() {
  int fdPointDAcces = creationPointDAcces();

  gestionPointDAcces(fdPointDAcces);

  return EXIT_SUCCESS;
}
