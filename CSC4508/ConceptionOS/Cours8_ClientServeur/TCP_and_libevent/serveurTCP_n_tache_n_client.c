/*********************************/
/* serveurTCP_n_tache_n_client.c */
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
#include <pthread.h>

#include "initCommunication.h"
#include "client-serveur.h"

//
// Constantes de fonctionnement du programme
//
//
#define NB_MAX_REPONSES 10 // Nombre maximum de reponses que le serveur fait
                           // a un client (avant de lui repondre REPONSE_KO) 

// Structure permettant de passer un entier en parametre lors
// d'un pthread_create
union IntPtr
{
    int  entier;
    void *ptr;
};

void *gestionConnexion(void *arg){
  union IntPtr ip;
  ip.ptr = arg;
  int fdConnexion = ip.entier;
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
  return NULL;
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

    pthread_t thread;
    union IntPtr ip;
    ip.entier = fdConnexion;
    int rc = pthread_create(&thread, NULL, gestionConnexion, ip.ptr);
    if (rc < 0)
      error_at_line(EXIT_FAILURE, rc, __FILE__, __LINE__, "pthread_create");
    rc = pthread_detach(thread);
    if (rc < 0)
      error_at_line(EXIT_FAILURE, rc, __FILE__, __LINE__, "pthread_detach");
  } while (1);
}

int main() {
  int fdPointDAcces = creationPointDAcces();

  gestionPointDAcces(fdPointDAcces);

  return EXIT_SUCCESS;
}
