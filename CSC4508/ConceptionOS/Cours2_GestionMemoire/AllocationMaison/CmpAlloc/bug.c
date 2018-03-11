// Programme destine a montrer un bug si on ne fait pas attention a.
// Ce sont les etudiants qui doivent trouver a quoi il faut faire attention.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "monAllocateur.h"

#define NBMAXALLOC 3
#define CHAINE "1234567890123456"

/**************************************************************************/
/* Programme principal                                                    */
/**************************************************************************/

int main(int argc, char* argv[]) {
  char *s1 = NULL;
  char *s2 = NULL;

  /* Initialisation */
  monAllocateurInit(NBMAXALLOC);

  /* C'est parti pour le bug */
  s1 = monMalloc();
  s2 = monMalloc();

  strcpy(s2, CHAINE);
  printf("s2 = \"%s\"\n", s2);
  strcpy(s1, CHAINE);
  printf("s1 = \"%s\"\n", s1);
  printf("s2 = \"%s\" ==> BUG !\n", s2);

  monFree(s1);
  s1 = NULL;
  monFree(s2);
  s2 = NULL;

  return EXIT_SUCCESS;
}


