#include <stdio.h>
#include <stdlib.h>

int varGlobale = 3; // var globale
static int varGlobaleStatic = 4; // var globale statique
extern int varGlobaleExterne; // var globale externe qui DOIT 

int main(int argc, char**argv) {
  
  int varLoc = 1; // var locale
  static int varLocStatic = 2; // var locale statique

  printf("Somme donne : %d\n", varLoc + varLocStatic);
  
  printf("varGlobale vaut %d, varGlobaleStatic vaut %d et varGLobaleExterne vaut %d", varGlobale, varGlobaleStatic, varGlobaleExterne);

  return EXIT_SUCCESS;
}
