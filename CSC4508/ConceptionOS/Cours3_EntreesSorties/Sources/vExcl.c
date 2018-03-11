/***********/
/* vExcl.c */
/***********/
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
int main(){
  int fd;
  struct flock verrou;

  fd = open("/tmp/ficTest",O_RDWR|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO);
  if (fd < 0) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  /* Verrou exclusif sur le 15è caractère */
  verrou.l_type = F_WRLCK;
  verrou.l_whence = SEEK_SET;
  verrou.l_start = 15;
  verrou.l_len = 1;
  /* A cause du parametre F_SETLKW, on se bloque sur le fcntl si jamais */
  /* le verrou ne peut pas etre pris                                   */
  printf("tentative de pose de verrou (exclusif) par processus %d...\n",
	 getpid());
  if (fcntl(fd, F_SETLKW, &verrou) < 0){
    perror("Pose verrou");
    exit(EXIT_FAILURE);
  }
  printf("... Verrou (exclusif) pose par processus %d\n", getpid());

  /* Ici on pourrait faire le traitement qui necessitait d'etre protege */
  /* par le verrou                                                      */
  sleep(10);

  /* Liberation du verrou */
  printf("Relachement verrou par processus %d...\n", getpid());
  verrou.l_type = F_UNLCK;
  verrou.l_whence = SEEK_SET;
  verrou.l_start = 15;
  verrou.l_len = 1;
  if (fcntl(fd, F_SETLK, &verrou) < 0){
    perror("Relachement verrou");
    exit(EXIT_FAILURE);
  }
  printf("...OK\n");

  return EXIT_SUCCESS;
}
