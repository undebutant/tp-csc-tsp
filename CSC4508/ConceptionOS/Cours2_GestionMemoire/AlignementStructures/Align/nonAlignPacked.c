#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "constantes.h"
#include "useGetrusage.h"

typedef struct {
  short	unShort;
  int	unInt;
  short	unShort2;
} __attribute__((packed)) tableElement;

tableElement t[SIZE_TABLE];

#define MEMBER_SIZEOF(aType,aMember) (sizeof( ((aType*)(0))->aMember ))

void printTableInfo() {
  printf("Taille d'un element de t = %zu\n", sizeof(tableElement));
  printf("Offset t[0].unShort  = %2zu (sizeof = %1zu)\n",
	 offsetof(tableElement,unShort),
	 MEMBER_SIZEOF(tableElement,unShort));
  printf("Offset t[0].unInt    = %2zu (sizeof = %1zu)\n",
	 offsetof(tableElement,unInt),
	 MEMBER_SIZEOF(tableElement,unInt));
  printf("Offset t[0].unShort2 = %2zu (sizeof = %1zu)\n",
	 offsetof(tableElement,unShort2),
	 MEMBER_SIZEOF(tableElement,unShort2));
  puts("");
}

int main(){
  int i;

  printTableInfo();

  getrusageStart();

  for (i=0 ; i<SIZE_TABLE ; i++) {
    t[i].unInt = UNE_VALEUR_QUELCONQUE;
  }

  getrusageStop();
  return EXIT_SUCCESS;
}
