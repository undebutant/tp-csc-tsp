/************************************/
/* serveurTCP_pool_tache_n_client.c */
/************************************/

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
#define NB_THREADS_DANS_POOL 1000 // Nombre de threads dans le pool

// Structure permettant de passer un entier en parametre lors
// d'un pthread_create
union IntPtr
{
    int  entier;
    void *ptr;
};

// Mutex evitant les race conditions sur rangClient
pthread_mutex_t mutex;

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

void *gestionPointDAcces(void *arg) {
  union IntPtr ip;
  ip.ptr = arg;
  int fdPointDAcces = ip.entier;
  static int rangClient = 0;
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
    if (pthread_mutex_lock(&mutex) != 0)
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "pthread_mutex_lock");    
    rangClient++;
    printf("%6d-ieme client a traiter\n", rangClient);
    if (pthread_mutex_unlock(&mutex) != 0)
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "pthread_mutex_unlock");

    gestionConnexion(fdConnexion);
  } while (1);
}

int main() {
  int fdPointDAcces = creationPointDAcces();
  int i;

  // Initialisation du mutex
  pthread_mutex_init(&mutex, NULL);

  // Creation du pool de threads
  for (i = 0 ; i < NB_THREADS_DANS_POOL-1 ; i++) {
    pthread_t thread;
    union IntPtr ip;
    ip.entier = fdPointDAcces;
    int rc = pthread_create(&thread, NULL, gestionPointDAcces, ip.ptr);
    if (rc < 0)
      error_at_line(EXIT_FAILURE, rc, __FILE__, __LINE__, "pthread_create");
    rc = pthread_detach(thread);
    if (rc < 0)
      error_at_line(EXIT_FAILURE, rc, __FILE__, __LINE__, "pthread_detach");
  }
  // Le thread du main est aussi un thread du pool
  // Si on ne fait pas ca, le thread main se finira, et ses fils finiront aussi
  // Des pthread join sont possibles aussi, mais plus si resultats et finition des fils
  // (pas le cas ici), ou on considere que le main peut aussi traiter des requetes
  //
  // Remarque : le union permet de considerer la variable, soit comme un entier, soit comme un pointeur
  // (sorte d'enum de type possiblepour une variable)
  union IntPtr ip;
  // La la var est vue comme un int
  ip.entier = fdPointDAcces;
  // Ici, comme un pointeur sur un int (au final equivalent)
  gestionPointDAcces(ip.ptr);

  return EXIT_SUCCESS;
}
