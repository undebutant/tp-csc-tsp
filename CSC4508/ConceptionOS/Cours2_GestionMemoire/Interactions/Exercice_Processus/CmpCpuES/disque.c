#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define NBFOIS 6

#define BSIZE  8192
#define FSIZE  500
#define FICNAME "/var/tmp/ficTest"

int main(){
  char buf[BSIZE];
  int fdes, i, j, rc;

  /* On cree NBFOIS un fichier de (FSIZE * BSIZE) octets */
  for (j=0 ; j<NBFOIS ; j++) {

    /* Ouverture du fichier */
    fdes = open(FICNAME, O_WRONLY|O_CREAT|O_SYNC|O_TRUNC, 0777);
    if (fdes < 0){
      perror("Pb lors de l'open");
      return EXIT_FAILURE;
    }

    /* Ecriture des (FSIZE * BSIZE) octets */
    for (i=0 ; i<FSIZE ; i++) {
      int nbWrite;
      nbWrite = write(fdes, buf, BSIZE);
      if (nbWrite < BSIZE) {
	if (nbWrite < 0) {
	  perror ("Pb lors du write");
	  return EXIT_FAILURE;
	}
	else {
	  printf("%d octets ecrits au lieu de %d (disque plein ?)\n",
		 nbWrite,
		 BSIZE);
	  return EXIT_FAILURE;
	}
      }
    }

    /* Fermeture du fichier */
    rc = close(fdes);
    if (rc < 0){
      perror("Pb lors du close");
      return EXIT_FAILURE;
    }
  }

  /* On supprime le fichier pour ne pas laisser de scories */
  rc = unlink(FICNAME);
  if (rc < 0){
    perror("Pb lors du unlink");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
