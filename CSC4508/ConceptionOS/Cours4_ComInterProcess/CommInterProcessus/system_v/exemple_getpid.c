/********************/
/* exemple_getpid.c */
/********************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  printf("pid = %d\n", getpid());
  printf("ppid = %d\n", getppid());
  printf("pgrp = %d\n", getpgrp());
  printf("création d'un groupe = %d\n", setpgrp());
  printf("pid = %d\n", getpid());
  printf("ppid = %d\n", getppid());
  printf("pgrp = %d\n\n", getpgrp());
  printf("uid = %d\n", getuid());
  printf("euid = %d\n", geteuid());
  printf("gid = %d\n", getgid());
  printf("egid = %d\n", getegid());
  printf("changement d'identité effective = %d, %d\n", setuid(0), setgid(0));
  printf("uid = %d\n", getuid());
  printf("euid = %d\n", geteuid());
  printf("gid = %d\n", getgid());
  printf("egid = %d\n", getegid());

  return EXIT_SUCCESS;
}
