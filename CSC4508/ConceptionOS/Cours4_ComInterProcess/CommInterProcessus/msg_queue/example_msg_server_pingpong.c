/****************************/
/* example_msg_serveur_pingpong.c */
/****************************/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>

#define TAILLEMSG 128
#define CLESERVEUR "/client-serveur"

void traiterRequete(char *requete){
  char reponse[TAILLEMSG];
  char typRequete[128];
  char cleClient[128];
  mqd_t mq_reponse;
  int nbWrite;

  /* Analyse de la requete */
  /* NB : sscanf presente l'inconvenient que si le client envoie */
  /* une chaine de caractere contenant un espace (par exemple :  */
  /* "PING PING") la chaine lue s'arrete au premier espace !     */
  /* fgets serait probablement meilleur.                         */
  sscanf(requete,"%s\n%s", typRequete, cleClient);

  /* Preparation de la reponse */
  sprintf(reponse, "%s", "PONG");

  /* Affichage */
  printf("Serveur a recu \"%s\" et repond \"%s\" sur le tube nomme \"%s\"\n",
	 typRequete,
	 reponse,
	 cleClient );
  /* Réponse sur le tube nommé du client */
  mq_reponse = mq_open(cleClient, O_WRONLY);
  if (mq_reponse < 0) {
    perror("mq_open");
    exit(EXIT_FAILURE);
  }

  nbWrite = mq_send(mq_reponse, reponse, sizeof(reponse), 1);
  if (nbWrite < 0) {
    perror("mq_send");
    exit(EXIT_FAILURE);
  }
  /* Dans cette application, le client ne renvoie pas de requête ultérieure */
  /* nécessitant une réponse ==> On peut fermer cette file de message       */
  if(mq_close(mq_reponse)<0) {
    perror("mq_close");
    exit(EXIT_FAILURE);
  }
}

int main() {
  char requete[TAILLEMSG];
  int nbRead;
  mqd_t requests;

  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;		/* size of the message queue */
  attr.mq_curmsgs = 0;
  attr.mq_msgsize = TAILLEMSG; /* size of one message */

  /* message queue for incoming requests */
  requests = mq_open(CLESERVEUR, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR, &attr);
  if (requests < 0) {
    perror("Server: mq_open");
    exit(EXIT_FAILURE);
  }

  /* Attente de requêtes */
  do{
    nbRead = mq_receive(requests, requete, sizeof(requete), NULL);
    if (nbRead != sizeof(requete)) {
      perror("mq_receive NOK");
      return EXIT_FAILURE;
    }
    /* On traite la requête */
    traiterRequete(requete);
  }while(1);

  if(mq_close(requests)<0) {
    perror("mq_close");
    return EXIT_FAILURE;
  }
  if(mq_unlink(CLESERVEUR)<0) {
    perror("mq_unlink");
    return EXIT_FAILURE;
  }
}
