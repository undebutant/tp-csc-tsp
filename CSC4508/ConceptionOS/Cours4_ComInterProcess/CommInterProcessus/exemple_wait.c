/******************/
/* exemple_wait.c */
/******************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void f(void) {
  printf("On va bientot sortir\n");
}

void g(void) {
  printf("On sort\n");
}

int main() {
  int pid, code_retour;
  pid = fork();
  if (pid) { /* parent */
    wait(&code_retour);
    code_retour = WEXITSTATUS(code_retour);
    printf("Parent : code de retour obtenu : %d\n", code_retour);
    return(code_retour);
  } else { /* enfant */
    atexit(g);
    atexit(f);
    printf("enfant : terminaison avec code de retour %d\n", 5);
    return 5;
  }
  return EXIT_SUCCESS;
}
