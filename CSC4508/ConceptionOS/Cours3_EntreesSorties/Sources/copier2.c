/*************/
/* copier2.c */
/*************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

FILE *source, *dest;
char buf[1024];
int nbElemLu, nbElemEcrit;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fputs("USAGE = copier2 nomSource nomDest\n", stderr);
    return EXIT_FAILURE;
  }
  source = fopen(argv[1], "r");
  if (source == NULL) {
    fprintf(stderr,
	    "Erreur '%s' lors ouverture fichier %s\n",
	    strerror(errno),
	    argv[1]);
    return EXIT_FAILURE;
  }
  dest = fopen(argv[2], "w");
  if (dest == NULL) {
    fprintf(stderr,
	    "Erreur '%s' lors ouverture fichier %s\n",
	    strerror(errno),
	    argv[2]);
    return EXIT_FAILURE;
  }
  while ((nbElemLu = fread((void*)buf, sizeof(buf[0]), sizeof(buf), source)) > 0) {
    nbElemEcrit = fwrite((void*)buf, sizeof(buf[0]), nbElemLu, dest);
    if (nbElemEcrit < nbElemLu) {
      fputs("Erreur d'ecriture\n", stderr);
      return EXIT_FAILURE;
    }
  }
  if (ferror(source) != 0) {
    fputs("Erreur fread", stderr);
    return EXIT_FAILURE;
  }
  if (fclose(source) < 0) {
    perror(argv[1]);
    return EXIT_FAILURE;
  }
  if (fclose(dest) < 0) {
    perror(argv[2]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
