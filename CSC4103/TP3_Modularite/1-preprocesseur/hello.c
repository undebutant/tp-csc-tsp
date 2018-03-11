#include <stdlib.h>
#include <stdio.h>

#define N 12

// extern int toto; variable déclarée, mais non définie ! Donc attend un module complémentaire pour la définir

int main()
{ int grandN=15;
  int n;

//  N = 7; Fautif car préprocesseur substuera N à 12, d'ou 12 = 7
  n = 7;
//  toto = 5; ne le trouve pas, not defined

  return 0;
}
