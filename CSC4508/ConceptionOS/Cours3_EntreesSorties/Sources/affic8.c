/************/
/* affic8.c */
/************/

/* Programme affichant 8 caracteres (a partir du i-ieme) du fichier dont */
/* le nom est fourni en parametre                                        */

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>

int main(int argc, char *argv[]) {
  int fd;
  int i,j;
  char *tab;
  int rc;

  if (argc < 3){
    fputs("USAGE = permut nomFic valeurDeI\n", stderr);
    return EXIT_FAILURE;
  }

  fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
    perror("open fic");
    return EXIT_FAILURE;
  }
  i = atoi(argv[2]);

  /* mmap */
  tab = (char *)mmap(0,
		     i+7,
		     PROT_READ,
		     MAP_PRIVATE,
		     fd,
		     0);
  if (tab == MAP_FAILED){
    perror("mmap");
    return EXIT_FAILURE;
  }

  /* Affichage */
  for (j=0 ; j<8 ; j++){
    putc(tab[i+j], stdout);
  }
  puts("");

  /* On ferme tout */
  rc = munmap(tab,i+7);
  if (rc < 0){
    perror("munmap");
    return EXIT_FAILURE;
  }
  if (close(fd) < 0){
    perror("close fic");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
