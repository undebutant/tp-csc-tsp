/****************/
/* clientTCP.c */
/****************/

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
//#define TEMPS_INTER_REQ 10000 // Temps entre 2 requetes (en micro-secondes)
#define TEMPS_INTER_REQ 1000000 // Temps entre 2 requetes (en micro-secondes)

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage = client identifiantClient\n");
    exit(EXIT_FAILURE);
  }

  char *idClient = argv[1];

  //
  // Connexion au serveur
  //
  int fdConnexion = connexionPointDAcces();
  printf("Client %6s : Connecte !\n", idClient);

  //
  // Envoi de requetes au serveur jusqu'a ce qu'il nous renvoie REQUETE_KO
  //
  printf("Client %6s : Envoi de requetes vers le serveur...\n", idClient);
  message_t msgReponse;
  int nbReponsesOK = 0;
  do {
    // Envoi requete
    message_t msgRequete;
    msgRequete.typ = REQUETE;
    strncpy(msgRequete.infoSup.idClient, idClient, TAILLE_ID_CLIENT);
    int nbWrite = write(fdConnexion, &msgRequete, sizeof(msgRequete));
    if (nbWrite < sizeof(msgRequete))
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "pb write sur tube nomme");

    // Lecture reponse
    int nbRead = read(fdConnexion, &msgReponse, sizeof(msgReponse));
    if (nbRead != sizeof(msgReponse))
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "Communication avec le serveur probablement rompue\n");
    if (msgReponse.typ == REQUETE_OK) {
      // Analyse reponse
      nbReponsesOK++;
      char payloadAttendue[TAILLE_PAYLOAD];
      CALCUL_PAYLOAD(payloadAttendue, idClient, nbReponsesOK);
      if (strcmp(payloadAttendue, msgReponse.infoSup.payload) == 0) {
	printf("Client %6s : Nouvelle reponse du serveur = \"%s\" ==> OK\n", idClient, msgReponse.infoSup.payload);
      } else {
	printf("Client %6s : Souci avec reponse du serveur !\n", idClient);
	printf("  - Attendu = \"%s\"\n", payloadAttendue);
	printf("  - Recu    = \"%s\"\n", msgReponse.infoSup.payload);
	printf("==> Arret !\n");
	exit(EXIT_FAILURE);
      }
    }
    usleep(TEMPS_INTER_REQ);
  } while (msgReponse.typ == REQUETE_OK);

  //
  // Ce client a termine son travail
  //
  printf("Client %6s : Envoi de requetes termine !\n", idClient);
  if (close(fdConnexion) < 0)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "close");

  return EXIT_SUCCESS;
}  
