#include <stdio.h>
#include <stdlib.h>

#ifdef M
#define N M // /!\ pas de = en #define, et si % def, N prend la même valeur
#endif
#ifndef N // si on met à la compilation -DN=7, N défini, on saute le define alors
/* si N n'est pas defini. On le definit et on lui donne la valeur 12 */
#define N 12
#endif

#if (N > 10)

void foo() {
  printf("N est très grand (%d)\n", N);
}

#elif (N<5)

void foo() {
  printf("N est très petit (%d)\n", N);
}

#else
void foo() {
  printf("N est très moyen (%d)\n", N);
}

#endif

int main(int argc, char**argv) {

  foo();
  return EXIT_SUCCESS;
}
