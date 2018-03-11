/* 
 * Programme simulant la gestion d'une imprimante
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


void impression(char *nomFic){

  printf("\nDebut d'impression de \"%s\"...\n", nomFic);
  sleep(5); /* Simule la lenteur de l'impression */
  printf("...Fin d'impression de \"%s\"...\n", nomFic);

}


void pere(int descPipe[2], char* nomFic) {
  printf("Bienvenue sur le serveur d'impression\n\n");

  do {
    printf("Nom du fichier a imprimer (taper '0' pour terminer) ? ");
    
    scanf("%s", nomFic);
    
    if(write(descPipe[1], nomFic, 80) != 80) {
      fprintf(stderr, "\nProblem with write, error returned: %s\n", strerror(errno));
      exit(1);
    }

    if (strcmp(nomFic,"0") == 0){
      printf("\nEnd of the print job launcher\n");
      break;
    }
  } while(1);
}


void fils(int descPipe[2], char *nomFic) {
  do {
    if(read(descPipe[0], nomFic, 80) < 0) {
      fprintf(stderr, "\nProblem with read, error returned: %s\n", strerror(errno));
      exit(1);
    }
    else if(strcmp(nomFic,"0") != 0) {
      impression(nomFic);
    }
    else {
      printf("End of the printer programm\n\n");
      break;
    }
  } while(1);
}


void attenteImpression(){
  char nomFic[80];

  int descPipe[2];
  int pipeRet = pipe(descPipe);
  if(pipeRet == -1) {
    fprintf(stderr, "\nError with pipe init, returned: %s\n", strerror(errno));
    exit(1);
  }

  int pid = fork();
  if(pid == -1) {
    fprintf(stderr, "\nFork failed, error returned: %s\n", strerror(errno));
    exit(1);
  }
  else if(pid == 0) {
    fils(descPipe, nomFic);
  }
  else {
    pere(descPipe, nomFic);
  }
}

int main(){

  attenteImpression();
  return EXIT_SUCCESS;

}
