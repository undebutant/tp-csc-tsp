/* Include(s) li�(s) � l'utilisation d'appels syst�me d'acc�s au fichier */
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Taille cible du fichier qu'on souhaite obtenir = 100 Mio              */
/* ATTENTION !                                                           */
/* Quand on parle de la capacit� d'un disque, 1Mo = 1.000.000 octets     */
/* En revanche, quand on parle de m�moire, 1 Mo = 1.048.576 octets       */
/* Pour �viter les ambigu�t�s, il vaudrait mieux utiliser la notation    */
/* Mio avec :                                                            */
/* 1 Mio (ou MMo) = 1024 Kio (ou KKo) = 1024 * 1024 octets =             */
/*                                         1<<20 octets = 1048576 octets */
/* (cf. http://physics.nist.gov/cuu/Units/binary.html)                   */
#define TAILLEFINALE (100 * (1<<20))

int main(int argc, char *argv[]) {
  /* D�claration de la variable contenant le descripteur de fichier      */
  /* manipul�                                                            */
  int fd;
  /* Autres d�clarations                                                 */
  size_t tailleBuf;
  char *buf;
  int i;

  if (argc != 2) {
    printf("USAGE = pw tailleBuffer\n");
    exit(1);
  }

  tailleBuf = atoi(argv[1]);

  buf = malloc(tailleBuf);
  memset(buf, 'A', tailleBuf);

  /* Ouverture du fichier "/var/tmp/ficTest"                               */
  /* (�criture seulement + cr�ation si n'existe pas +                      */
  /* si le fichier existe, il est tronqu� ; pouvant �tre lu et �crit par   */
  /* par l'utilisateur (les autres n'ayant aucun droit)                    */
  fd = open("/var/tmp/fictest", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
//  fd = open("/var/tmp/fictest", O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, S_IRUSR | S_IWUSR);
  assert(fd >=0);
  /* Ecriture du fichier par blocs de tailleBuf                            */
  for (i=0 ; i<TAILLEFINALE/tailleBuf ; i++) {
    /* Ecriture d'un bloc buf dans le fichier                              */
    int sizeWrote = write(fd, (void*)buf, tailleBuf);
    assert(sizeWrote == tailleBuf);
  }

  /* Fermeture du fichier                                                  */
  assert(close(fd) == 0);

  return EXIT_SUCCESS;
}
