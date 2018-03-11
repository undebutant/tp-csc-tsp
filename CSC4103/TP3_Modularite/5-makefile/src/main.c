#include <stdlib.h>
#include <stdio.h>
#include "foo.h"
#include "tab.h"

int main(int argc, char**argv) {
  int tab[3];
  TYPE n = foo();

  saisir_tab(tab, 3);
  
  printf("%d\n", n);
  
  return EXIT_SUCCESS;
}
