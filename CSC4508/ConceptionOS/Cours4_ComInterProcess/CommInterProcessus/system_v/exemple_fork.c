/******************/
/* exemple_fork.c */
/******************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t pid;
  printf("démonstration de fork\n");
  pid = fork();
  switch (pid) {
  case -1 :
    fprintf(stderr, "echec de fork\n");
    return EXIT_FAILURE;
  case 0 : /* enfant */
    sleep(2);
    printf("je suis l'enfant\n");
    printf("pid = %d\n", getpid());
    printf("ppid = %d\n", getppid());
    printf("pgrp = %d\n", getpgrp());
    printf("uid = %d\n", getuid());
    printf("euid = %d\n", geteuid());
    printf("gid = %d\n", getgid());
    break;
  default : /* parent */
    printf("je suis le parent\n");
    printf("pid = %d\n", getpid());
    printf("ppid = %d\n", getppid());
    printf("pgrp = %d\n", getpgrp());
    printf("uid = %d\n", getuid());
    printf("euid = %d\n", geteuid());
    printf("gid = %d\n", getgid());
    printf("pid de l'enfant= %d\n", pid);
    break;
  }
  return EXIT_SUCCESS;
}
