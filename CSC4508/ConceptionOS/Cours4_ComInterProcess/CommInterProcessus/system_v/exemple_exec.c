/******************/
/* exemple_exec.c */
/******************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int code_retour;
  system("ps");
  printf("pid = %d\n", getpid());
  printf("ppid = %d\n", getppid());
  printf("pgrp = %d\n", getpgrp());
  printf("uid = %d\n", getuid());
  printf("euid = %d\n", geteuid());
  printf("gid = %d\n", getgid());
  code_retour = execlp("ps", "ps", (char *)NULL);
  printf("code_retour vaut %d\n",code_retour);
  perror("pb sur exec");
  printf("fin du programme appelant\n");
  return EXIT_FAILURE;
}
