/********************/
/* exemple_getenv.c */
/********************/

#include <stdio.h>
#include <stdlib.h>
extern char **environ;
int main(int argc, char **argv, char **envp) {
  char *p_shell;
  char **ancien_environ = environ;
  /* parcours de l'environnement */
  printf("---> envp :\n");
  while(*envp != NULL) {
    printf("%s\n", *envp);
    envp++;
  }
  printf("---> environ :\n");
  while(*environ != NULL) {
    printf("%s\n", *environ);
    environ++;
  }
  /* affichage de la variable SHELL si elle existe */
  printf("---> recherche de la variable SHELL :\n");
  if ((p_shell = getenv("SHELL")) != NULL) {
    printf("SHELL : %s\n", p_shell);
  } else {
    printf("variable SHELL non définie\n");
  }
  environ = ancien_environ;
  printf("---> recherche de la variable SHELL (environ réinitialisée):\n");
  if ((p_shell = getenv("SHELL")) != NULL) {
    printf("SHELL : %s\n", p_shell);
  } else {
    printf("variable SHELL non définie\n");
  }
  return EXIT_SUCCESS;
}
