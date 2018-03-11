/***************************/
/* exemple_msg_client_pingpong.c */
/***************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>

#define TAILLEMSG 128
#define CLESERVEUR "/client-serveur"

int main() {
  char cleClient[128];
  char requete[TAILLEMSG];
  char reponse[TAILLEMSG];
  mqd_t mq_requete, mq_reponse;
  int nbRead, nbWrite;

  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;		/* size of the message queue */
  attr.mq_curmsgs = 0;
  attr.mq_msgsize = TAILLEMSG; /* size of one message */

  /* Pr�paration de cleClient (file de message serveur->client */
  sprintf(cleClient, "/serveur-client.%d", getpid());
  /* Cr�ation du tube nomm� qui permettra au serveur de repondre au client */
  mq_reponse = mq_open(cleClient, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR, &attr);
  if (mq_reponse < 0) {
    perror("mq_open");
    exit(EXIT_FAILURE);
  }

  /* ouverture de la file de message client->serveur */
  mq_requete = mq_open(CLESERVEUR, O_WRONLY);
  if (mq_requete < 0) {
    perror("mq_open");
    exit(EXIT_FAILURE);
  }

  /* Envoi de la requ�te vers le serveur (la requete contient le nom du */
  /* tube sur lequel la reponse est attendue)                           */
  sprintf(requete,"%s\n%s", "PING", cleClient);
  nbWrite = mq_send(mq_requete, requete, sizeof(requete), 1);
  if (nbWrite < 0) {
	perror("mq_send");
  }

  /* On n'aura pas d'autres messages � envoyer au serveur. On peut donc    */
  /* fermer le tube cote emission                                          */
  if(mq_close(mq_requete)<0) {
    perror("mq_close");
    return EXIT_FAILURE;
  }

  /* On lit la r�ponse et on l'affiche */
  nbRead = mq_receive(mq_reponse, reponse, sizeof(reponse), NULL);
  if (nbRead != sizeof(reponse)) {
    printf("Communication avec le serveur probablement rompue\n");
    exit(EXIT_FAILURE);
  }
  printf("Reponse du serveur = \"%s\"\n", reponse);
  /* On ferme le tube c�t� r�ception */
  if(mq_close(mq_reponse)<0) {
    perror("mq_close");
    return EXIT_FAILURE;
  }
  /* On d�truit la file de message du client (puisqu'elle ne servira plus */
  /* jamais.                                                              */
  if(mq_unlink(cleClient)<0) {
    perror("mq_unlink");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
