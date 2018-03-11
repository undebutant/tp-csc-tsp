/************************/
/* exemple_msg_client.c */
/************************/

/* Processus Client : appel : client texteSansEspace
   Resultat : affichage du texte renvoye en echo par le serveur
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MSGREQ 10	/* file des requetes */
#define MSGREP 20	/* file des reponses */

struct msgforme {
  long type;
  char texte[256];
};

int main(int argc, char **argv) {
  int msgreq,msgrep;
  struct msgforme rep, req;
  if (argc != 2) {
    printf("Client : Usage : %s texteSansEspace\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  if ((msgreq = msgget(MSGREQ, 0)) == -1
      || (msgrep = msgget(MSGREP, 0)) == -1) {
    perror("Client : msgget");
    exit(EXIT_FAILURE);
  }
  strcpy(req.texte, argv[1]);
  req.type = getpid(); /* type des messages = pid */
  /* envoi requete */
  if (msgsnd(msgreq, &req, strlen(req.texte) +  + sizeof('\0'), 0) == -1) {
    perror("Client : msgsnd");
    exit(EXIT_FAILURE);
  }
  /* attente reponse d'un message de type = pid */
  if (msgrcv(msgrep, &rep, sizeof(rep.texte), getpid(), 0) == -1) {
    perror("Client : msgrcv");
    exit(EXIT_FAILURE);
  }
  printf("Client : message emis %d : %s \n",
	 getpid(), req.texte);
  printf("Client : message recu %d : %s \n",
	 getpid(), rep.texte);
  return EXIT_SUCCESS;
}
