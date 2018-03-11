/***************************/
/* exemple_mkfifo_client.c */
/***************************/

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define TAILLEMSG 128
#define NOMTUBESERVEUR "./client-serveur"

int main() {
  char nomTubeClient[128];
  char requete[TAILLEMSG];
  char reponse[TAILLEMSG];
  int fdW, fdR;
  int nbRead, nbWrite;

  /* Préparation de nomTubeClient (tube qui permettra les communications */
  /* serveur-client)                                                     */
  sprintf(nomTubeClient, "/tmp/serveur-client.%d", getpid());
  /* Création du tube nommé qui permettra au serveur de repondre au client */
  if (mkfifo(nomTubeClient, S_IRUSR|S_IWUSR) < 0) {
    if (errno != EEXIST) {
      perror("mkfifo(tube nommé client");
      exit(EXIT_FAILURE);
    }
    else {
      printf("%s existe deja : on suppose que c'est un pipe nomme\n",
	     nomTubeClient );
      printf("et qu'on peut continuer le programme sans probleme\n");
      puts("");
    }
  }
  /* Ouverture de ce tube */
  fdR = open(nomTubeClient, O_RDWR);
  if (fdR == -1) {
    perror("open(tube nommé)");
    exit(EXIT_FAILURE);
  }
  /* Envoi de la requête vers le serveur (la requete contient le nom du */
  /* tube sur lequel la reponse est attendue)                           */
  sprintf(requete,"%s\n%s", "PING", nomTubeClient);
  fdW = open(NOMTUBESERVEUR, O_WRONLY);
  if (fdW == -1) {
    perror("open(NOMTUBESERVEUR)");
    exit(EXIT_FAILURE);
  }
  nbWrite = write(fdW, requete, sizeof(requete));
  if (nbWrite < sizeof(requete)) {
	perror("pb ecriture sur pipe nomme");
  }
  /* On n'aura pas d'autres messages à envoyer au serveur. On peut donc    */
  /* fermer le tube cote emission                                          */
  close(fdW);
  /* On lit la réponse et on l'affiche */
  nbRead = read(fdR, reponse, sizeof(reponse));
  if (nbRead != sizeof(reponse)) {
    printf("Communication avec le serveur probablement rompue\n");
    exit(EXIT_FAILURE);
  }
  printf("Reponse du serveur = \"%s\"\n", reponse);
  /* On ferme le tube côté réception */
  close(fdR);
  /* On détruit le tube nommé du client (puisque ce tube nommé ne servira */
  /* plus jamais.                                                         */
  if (unlink(nomTubeClient) < 0) {
    perror("unlink");
    exit(EXIT_FAILURE);
  }
  return EXIT_SUCCESS;
}
