/****************************/
/* exemple_mkfifo_serveur.c */
/****************************/

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

void traiterRequete(char *requete){
  char reponse[TAILLEMSG];
  char typRequete[128];
  char nomTubeClient[128];
  int fdW;
  int nbWrite;

  /* Analyse de la requete */
  /* NB : sscanf presente l'inconvenient que si le client envoie */
  /* une chaine de caractere contenant un espace (par exemple :  */
  /* "PING PING") la chaine lue s'arrete au premier espace !     */
  /* fgets serait probablement meilleur.                         */
  sscanf(requete,"%s\n%s", typRequete, nomTubeClient);
  /* Preparation de la reponse */
  sprintf(reponse, "%s", "PONG");
  /* Affichage */
  printf("Serveur a recu \"%s\" et repond \"%s\" sur le tube nomme \"%s\"\n",
	 typRequete,
	 reponse,
	 nomTubeClient );
  /* Réponse sur le tube nommé du client */
  fdW = open(nomTubeClient, O_WRONLY);
  if (fdW == -1) {
    perror("open(nomTubeClient)");
    exit(EXIT_FAILURE);
  }
  nbWrite = write(fdW, reponse, sizeof(reponse));
  if (nbWrite < sizeof(reponse)) {
    perror("pb ecriture sur tube nomme");
  }
  /* Dans cette application, le client ne renvoie pas de requête ultérieure*/
  /* nécessitant une réponse ==> On peut fermer ce tube                    */
  close(fdW);
}

int main() {
  char requete[TAILLEMSG];
  int fdR;
  int nbRead;

  /* Création du tube nommé côté serveur (qui permettra les communications */
  /* client-serveur)                                                       */
  if (mkfifo(NOMTUBESERVEUR, S_IRUSR|S_IWUSR) < 0) {
    if (errno != EEXIST) {
      perror("mkfifo(tube nommé serveur");
      exit(EXIT_FAILURE);
    }
    else {
      printf("%s existe deja : on suppose que c'est un pipe nomme\n",
	     NOMTUBESERVEUR);
      printf("et qu'on peut continuer le programme sans probleme\n");
      puts("");
    }
  }
  /* Ouverture de ce tube nommé */
  fdR = open(NOMTUBESERVEUR, O_RDWR);
  if (fdR == -1) {
    perror("open(tube nommé)");
    exit(EXIT_FAILURE);
  }
  /* Attente de requêtes */
  do{
    nbRead = read(fdR, requete, sizeof(requete));
    if (nbRead != sizeof(requete)) {
      perror("read sur tube nomme NOK");
      return EXIT_FAILURE;
    }
    /* On traite la requête */
    traiterRequete(requete);
  }while(1);
}
