/*************************/
/* exemple_msg_serveur.c */
/*************************/

/* Processus Serveur : appel : server&
   Resultat : echo des requetes soumises par les clients
   Arret : sur reception d'un message commencant par @
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#define MSGREQ 10
#define MSGREP 20

struct msgforme {
  long type;
  char texte[256];
};

int main() {
  int msgrep, msgreq, i;
  struct msgforme req, rep;
  /* file des requetes */
  if ((msgreq = msgget(MSGREQ, 0600|IPC_CREAT)) < 0) {
    perror("Serveur : msget");
    exit(EXIT_FAILURE);
  }
  /* file des reponses */
  if ((msgrep = msgget(MSGREP, 0600|IPC_CREAT)) < 0) {
    perror("Serveur : msget");
    exit(EXIT_FAILURE);
  }
  for (;;) {
    /* attente requete de n'importe quel type */
    if (msgrcv(msgreq, &req, sizeof(req.texte), 0, 0) < 0) {
      perror("Serveur : msgrcv");
      exit(EXIT_FAILURE);
    }
    printf("Serveur : message recu %ld : %s \n",
	   req.type, req.texte);
    rep.type = req.type; /* type de la reponse = PID demandeur */
    if (req.texte[0] == '@') { /* test fin */
      break;
    }
    for (i = 0; req.texte[i] != 0; i++) {
      rep.texte[i] = req.texte[i];
    }
    rep.texte[i] = '\0';
    if (msgsnd(msgrep, &rep, strlen(rep.texte) + 1, 0) < 0) {
      perror("Serveur : msgsnd");
      exit(EXIT_FAILURE);
    } 
    printf("Serveur : message emis %ld : %s \n",
	   rep.type, rep.texte);
  }
  strcpy(rep.texte, "Fin du serveur");
  if (msgsnd(msgrep, &rep, strlen(rep.texte) + sizeof('\0'), 0) < 0) {
    perror("Serveur : msgsnd");
    exit(EXIT_FAILURE);
  } 
  sleep(10);
  if (msgctl(msgreq, IPC_RMID, 0) < 0) { /* suppression */
    perror("Serveur : msgctl");
    exit(EXIT_FAILURE);
  } 
  if (msgctl(msgrep, IPC_RMID, 0) < 0) { /* suppression */
    perror("Serveur : msgctl");
    exit(EXIT_FAILURE);
  } 
  return EXIT_SUCCESS;
}
