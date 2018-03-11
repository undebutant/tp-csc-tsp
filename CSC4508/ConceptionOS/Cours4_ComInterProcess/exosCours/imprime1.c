/* 
 * Programme simulant la gestion d'une imprimante
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void impression(char *nomFic){

  printf("Debut d'impression de \"%s\"...\n", nomFic);
  sleep(5); /* Simule la lenteur de l'impression */
  printf("...Fin d'impression de \"%s\"...\n", nomFic);

}

void attenteImpression(){
  char nomFic[80];

  do {
    printf("Nom du fichier a imprimer (taper '0' pour terminer) ? ");
    scanf("%s", nomFic);
    if (strcmp(nomFic,"0") == 0){
      break;
    }

      int pid = fork();
      if(pid == -1) {
	perror("\nFork failed, ");
	fprintf(stderr, "error returned: %s\n", strerror(errno));
      }
      else if(!pid) {
	impression(nomFic);
	return;
      }

  }while (1);
  printf("Au revoir\n");

}

int main(){

  attenteImpression();
  return EXIT_SUCCESS;

}
