/***************/
/* tailleFic.c */
/***************/

/* Programme affichant la taille en octet du fichier dont le nom est */
/* fourni en parametre                                               */

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  int fd;
  struct stat sts;

  if (argc < 2){
    fputs("USAGE = tailleFic nomFic\n", stderr);
    return EXIT_FAILURE;
  }

  fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
    perror("open fic");
    return EXIT_FAILURE;
  }
  if (fstat(fd,&sts) < 0) {
    perror("fstat");
    return EXIT_FAILURE;
  }
  printf("Fichier %s a %ld octets\n", argv[1], sts.st_size);
  if (close(fd) < 0){
    perror("close fic");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
