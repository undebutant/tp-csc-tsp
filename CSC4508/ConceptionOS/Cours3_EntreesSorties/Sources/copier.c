/************/
/* copier.c */
/************/
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define USAGE "USAGE = copier nomSource nomDest\n"
#define ERRECRIT "Erreur d'ecriture (disque plein ?)\n"

int source, dest;
int buf;
int nbCarLu, nbCarEcrit;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    write(STDERR_FILENO, USAGE, strlen(USAGE));
    return EXIT_FAILURE;
  }
  source = open(argv[1], O_RDONLY);
  if (source < 0) {
    perror(argv[1]);
    return EXIT_FAILURE;
  }
  dest = open(argv[2],
              O_WRONLY|O_CREAT|O_TRUNC,
              S_IRWXU|S_IRWXG|S_IRWXO);
  if (dest < 0) {
    perror(argv[2]);
    return EXIT_FAILURE;
  }
  while ((nbCarLu = read(source, (void*)&buf, sizeof(buf))) > 0) {
    nbCarEcrit = write(dest, (void*)&buf, nbCarLu);
    if (nbCarEcrit <= 0) {
      if (nbCarEcrit == 0) {
        write(STDERR_FILENO, ERRECRIT, strlen(ERRECRIT));
      }
      else {
        perror("write");
      }
      return EXIT_FAILURE;
    }
  }
  if (nbCarLu < 0) {
    perror("read");
    return EXIT_FAILURE;
  }
  if (close(source) < 0) {
    perror(argv[1]);
    return EXIT_FAILURE;
  }
  if (close(dest) < 0) {
    perror(argv[2]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
