#include <mqueue.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>

#include "client-serveur.h"

int main(int argc, char *argv[]) {
  requete_t requete;
  reponse_t reponse;
  mqd_t mqRequetes;
  mqd_t mqReponses;
  struct mq_attr attr;
  int i;
  int len;

  if (argc != 2) {
    printf("Usage = client nomDeFamilleCherche\n");
    exit(EXIT_FAILURE);
  }

  /* Preparation du champ pointAccesClient de la requete */
  sprintf(requete.pointAccesClient, "/serveur-client.%d", getpid());

  /* Preparation du champ contenu : On y recopie argv[1] en mettant en */
  /* majuscules                                                        */
  len = strlen(argv[1]);
  for (i=0 ; i < len ; i++) {
    requete.contenu[i] = toupper(argv[1][i]);
  }
  requete.contenu[len] = '\0';

  /* Creation de la message queue pour envoyer les requetes */
  mqRequetes = mq_open(NOM_POINT_ACCES_SERV, O_WRONLY);
  if (mqRequetes < 0) {
    perror("Client: mq_open");
    exit(EXIT_FAILURE);
  }

  /* Creation de la message queue pour recevoir les reponses */
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;		/* size of the message queue */
  attr.mq_curmsgs = 0;
  attr.mq_msgsize = sizeof(reponse_t); /* size of one message */
  mqReponses = mq_open(requete.pointAccesClient, O_CREAT|O_RDONLY, S_IRUSR|S_IWUSR, &attr);
  if (mqReponses < 0) {
    perror("Client: mq_open");
    exit(EXIT_FAILURE);
  }

  /* Envoi de la requete vers le serveur */
  len = mq_send(mqRequetes, (char*)&requete, sizeof(requete), 1);
  if (len < 0) {
    perror("Client: mq_send");
    exit(EXIT_FAILURE);
  }
  printf("Client: message sent: %s \n", requete.contenu);

  /* On lit la reponse */
  len = mq_receive(mqReponses, (char*)&reponse, sizeof(reponse), NULL);
  if(len < sizeof(reponse)) {
    if (len < 0) {
      perror("Client: mq_receive");
    } else {
      fprintf(stderr, "Bizarre : on n'a recu que %d octets alors qu'on en attendait %zu !\n", len, sizeof(reponse));
    }
    exit(EXIT_FAILURE);
  }

  /* On affiche la reponse */
  printf("Reponse du serveur = \"%s\"\n", reponse.contenu);

  /* NB : Dans cette application, on utilise pas le point d'acces prive */
  /* vers le serveur                                                    */

  /* On ferme les message queues dont on n'a plus besoin */
  if(mq_close(mqRequetes) < 0){
    perror("Client: mq_close");
    exit(EXIT_FAILURE);
  }
  if(mq_close(mqReponses) < 0){
    perror("Client: mq_close");
    exit(EXIT_FAILURE);
  }

  /* On supprime les message queues dont on n'a plus besoin */
  if(mq_unlink(requete.pointAccesClient) < 0){
    perror("Client: mq_close");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}


   
