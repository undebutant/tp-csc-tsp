/******************/
/* exemple_pipe.c */
/******************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void parent(int tube, char *message) {
  sprintf(message, "Message du parent (%d)\n", getpid());
  write(tube, message, strlen(message) + 1);
  close(tube);
}

void enfant(int tube) {
  char boite;
  printf("Message reçu par %d : \"", getpid());
  while (read(tube, &boite, 1) == 1)
    putchar(boite);
  printf("\"\n\n");
}

int main() {
  int pid, descfich[2], code_retour;
  char tampon[80];
  pipe(descfich);
  pid = fork();
  switch (pid) {
  case -1 :
    fprintf(stderr, "echec de fork\n");
    exit(EXIT_FAILURE);
  case 0 : /* enfant */
    close(descfich[1]);  /* ... n'ecrit pas dans le tube mais lit */
    enfant(descfich[0]);
    break;
  default : /* parent */
    close(descfich[0]);  /* ... ne lit pas dans le tube mais ecrit */
    parent(descfich[1], tampon);
    wait(&code_retour);
    break;
  }
  return EXIT_SUCCESS;
}
