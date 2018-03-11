// Programme destine a comparer les performances de malloc et les performances
// d'un mecanisme d'allocation base sur une liste chainee de blocs de meme
// taille

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sched.h>
#include <assert.h>

#include "monAllocateur.h"

#define NBMAXALLOC 100000

typedef char bloc_t[TAILLEBLOC];

int nbAlloc = 0;
bloc_t *blocAlloues[NBMAXALLOC + 1];


/**************************************************************************/
/* Effectue aleatoirement nbOper operations d'allocations/desallocations  */
/* avec malloc/free, le generateur aleatoire etant initialise avec        */
/* graineSrand                                                            */
/**************************************************************************/
void travailAvecMalloc(int nbOper,
		       int graineSrand)
{
  int i;

  /* Initialise le generateur aleatoire */
  srand(graineSrand);

  for (i=0 ; i <= NBMAXALLOC ; ++i) {
    blocAlloues[i] = NULL;
  }

  /* C'est parti pour nbOper d'allocation/desallocation */
  for (i=nbOper ; i>0 ; --i) {
    int valAlea = (rand()%100) + 1;
    if (valAlea > (nbAlloc*100/NBMAXALLOC)) {
      /* On fait une nouvelle allocation */
      assert(nbAlloc<NBMAXALLOC); /* Si nbAlloc >= NBMAXALLOC, valAlea */
                                  /* est forcement <=                  */
                                  /* nbAlloc*100/NBMAXALLOC ==> on ne  */
                                  /* pas arriver ici.                  */

      assert(blocAlloues[nbAlloc] == NULL);
      blocAlloues[nbAlloc] = malloc(TAILLEBLOC);
      assert(blocAlloues[nbAlloc] != NULL);
      nbAlloc++;
    }
    else {
      int numBlocALiberer;
      /* On fait une desallocation */
      assert(nbAlloc > 0); /* Si nbAlloc est nul ou negatif, valAlea lui*/
                           /* est forcement superieur ==> on ne peut    */
                           /* arriver dans ce else avec nbAlloc <= 0    */

      /* On tire au hasard le bloc qu'on va liberer */
      numBlocALiberer = rand() % nbAlloc;
      free((void *)blocAlloues[numBlocALiberer]);

      /* On supprime la reference a ce bloc */
      nbAlloc--;
      blocAlloues[numBlocALiberer] = blocAlloues[nbAlloc];
      blocAlloues[nbAlloc] = NULL;
    }
  }

  /* Il nous reste a liberer toute la memoire */
  for (i=nbAlloc-1 ; i>=0 ; --i){
    free((void *)blocAlloues[i]);
  }
  nbAlloc = 0;
    
}
  

/**************************************************************************/
/* Effectue aleatoirement nbOper operations d'allocations/desallocations  */
/* avec monMalloc/monFree, le generateur aleatoire etant initialise avec  */
/* graineSrand                                                            */
/**************************************************************************/
void travailAvecMonMalloc(int nbOper,
			  int graineSrand)
{
  int i;

  /* Initialise le generateur aleatoire */
  srand(graineSrand);

  for (i=0 ; i <= NBMAXALLOC ; ++i) {
    blocAlloues[i] = NULL;
  }

  /* C'est parti pour nbOper d'allocation/desallocation */
  for (i=nbOper ; i>0 ; --i) {
    int valAlea = (rand()%100) + 1;
    if (valAlea > (nbAlloc*100/NBMAXALLOC)) {
      /* On fait une nouvelle allocation */
      assert(nbAlloc<NBMAXALLOC); /* Si nbAlloc >= NBMAXALLOC, valAlea */
                                  /* est forcement <=                  */
                                  /* nbAlloc*100/NBMAXALLOC ==> on ne  */
                                  /* pas arriver ici.                  */

      blocAlloues[nbAlloc] = (bloc_t *)monMalloc();
      nbAlloc++;
    }
    else {
      int numBlocALiberer;
      /* On fait une desallocation */
      assert(nbAlloc > 0); /* Si nbAlloc est nul ou negatif, valAlea lui*/
                           /* est forcement superieur ==> on ne peut    */
                           /* arriver dans ce else avec nbAlloc <= 0    */

      /* On tire au hasard le bloc qu'on va liberer */
      numBlocALiberer = rand() % nbAlloc;
      monFree((void *)blocAlloues[numBlocALiberer]);

      /* On supprime la reference a ce bloc */
      nbAlloc--;
      blocAlloues[numBlocALiberer] = blocAlloues[nbAlloc];
      blocAlloues[nbAlloc] = NULL;
    }
  }

  /* Il nous reste a liberer toute la memoire */
  for (i=nbAlloc-1 ; i>=0 ; --i){
    monFree((void *)blocAlloues[i]);
  }
  nbAlloc = 0;

}
  

/**************************************************************************/
/* Programme principal                                                    */
/**************************************************************************/
int main(int argc, char* argv[]) {

  int nbOper;
  int graineSrand;

  if (argc != 3) {
    fprintf(stderr, "USAGE = cmpAlloc nbOper graineSrand\n");
    return EXIT_FAILURE;
  }

  nbOper = atoi(argv[1]);
  graineSrand = atoi(argv[2]);

  /* Initialisation */
  monAllocateurInit(NBMAXALLOC);

  /* Comparaison */
  travailAvecMalloc(nbOper, graineSrand);
  travailAvecMonMalloc(nbOper, graineSrand);

  return EXIT_SUCCESS;
}


