#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "constantes.h"
#include "useGetrusage.h"

#ifdef ALIGN
typedef struct {
  char octetsAvant[FRONTIERE_TESTEE-sizeof(int)];
  int	unInt;
  char octetsApres[FRONTIERE_TESTEE];
} __attribute__((packed)) tableElement;
#else
typedef struct {
  char octetsAvant[FRONTIERE_TESTEE-sizeof(int)/2];
  int	unInt;
  char octetsApres[FRONTIERE_TESTEE-sizeof(int)/2];
} __attribute__((packed)) tableElement;
#endif

tableElement t[SIZE_TABLE] __attribute__((aligned(FRONTIERE_TESTEE)));

#define MEMBER_SIZEOF(aType,aMember) (sizeof( ((aType*)(0))->aMember ))

#define NB_FOIS 10

void printTableInfo() {
  printf("FRONTIERE_TESTEE = %d\n", FRONTIERE_TESTEE);
  printf("Taille d'un element de t = %zu\n", sizeof(tableElement));
  printf("Offset t[0].octetsAvant  = %2zu (sizeof = %1zu)\n",
	 offsetof(tableElement,octetsAvant),
	 MEMBER_SIZEOF(tableElement,octetsAvant));
  printf("Offset t[0].unInt    = %2zu (sizeof = %1zu)\n",
	 offsetof(tableElement,unInt),
	 MEMBER_SIZEOF(tableElement,unInt));
  printf("Offset t[0].octetsApres = %2zu (sizeof = %1zu)\n",
	 offsetof(tableElement,octetsApres),
	 MEMBER_SIZEOF(tableElement,octetsApres));
  printf("Adresse du tableau = %p\n", t);
  puts("");
}

int main(){
  int i,j;

  printTableInfo();

  getrusageStart();

  for (j=NB_FOIS ; j>0 ; j--) {
    for (i=SIZE_TABLE ; i>0 ; i--) {
      t[i].unInt = UNE_VALEUR_QUELCONQUE;
    }
  }

  printf("Nombre d'acces = %d\n", NB_FOIS*SIZE_TABLE);

  getrusageStop();
  return EXIT_SUCCESS;
}
