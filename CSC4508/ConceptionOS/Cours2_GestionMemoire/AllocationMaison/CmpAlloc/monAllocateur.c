/* Allocateur base sur la constitution d'une liste chaine de bloc pointee */
/* par debutListe                                                         */

/*********************************************
Explications liees au fonctionnement bugge du programme bug.c

1) Quelle est l'origine du bug ?
* A completer au moment de la question 2 *

2) Pour quelle raison valgrind applique au programme bug ne detecte rien ?
* A completer au moment de la question 2 *

3) Quelle fonction utilisee en lieu et place de strcpy aurait évite ce bug ?
* A completer au moment de la question 2 *

d. Complétez les informations suivantes : * A completer au moment de la question *3* *
- Type machine utilisée (si vous le connaissez) :
- Processeur et fréquence (si vous les connaissez) :
- Taille RAM (si vous la connaissez) :
- Système d'exploitation (uname -a) :
- Synthèse des résultats :

                        +-------------------------+-------------------------------+--------------------+
                        | par appel a malloc/free | par appel a monMalloc/monFree | Reduction du temps |
                        |     (nanosecondes)      |         (nanosecondes)        |    (pourcentage)   |
+-----------------------+-------------------------+-------------------------------+--------------------+
| Allocation mémoire    |                us       |                       us      |               %    |    
+-----------------------+-------------------------+-------------------------------+--------------------+
| Désallocation mémoire |                us       |                       us      |               %    |
+-----------------------+-------------------------+-------------------------------+--------------------+

*********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "monAllocateur.h"

typedef struct Bloc {
	void *nextBlocPtr;
	uint8_t data[TAILLEBLOC - sizeof(struct Bloc*)];
} Bloc;

static Bloc *debutListe;

/**************************************************************************/
/* Routine d'initialisation de l'allocateur : constitue une liste chainee */
/* de nbMaxAlloc blocs de taille TAILLEBLOC, le premier bloc etant pointe */
/* par debutListe et le dernier pointant sur NULL                         */
/**************************************************************************/
void monAllocateurInit(int nbMaxAlloc) {
	/* On fait le Malloc global */
	void * buffer = malloc (nbMaxAlloc * sizeof(struct Bloc));
	if(!buffer) {
		fprintf(stderr, "Malloc failed!\n");
		abort();
	}

	/* On definit la liste chainee */
	debutListe = buffer;

	struct Bloc* cur_bloc = debutListe;
	struct Bloc* next_bloc;

	for (int i = 0 ; i < nbMaxAlloc - 1 ; i++) {
		next_bloc = cur_bloc + 1;
		cur_bloc->nextBlocPtr = next_bloc;

		cur_bloc = cur_bloc->nextBlocPtr;
	}

	cur_bloc->nextBlocPtr = NULL;
}

/**************************************************************************/
/* Fonction renvoyant un pointeur sur une zone memoire                    */
/* NB : on ne peut pas preciser la taille, puisque la taille est          */
/*      predefinie                                                        */
/**************************************************************************/
void *monMalloc(void) {
	void *allocUser = debutListe;
	if(debutListe) {
		debutListe = debutListe->nextBlocPtr;
	}
	return allocUser;
}

/**************************************************************************/
/* Fonction liberant la zone memoire pointee par ptr                      */
/**************************************************************************/
void monFree(void *ptr) {
	struct Bloc *newlyFreeBloc = ptr;
	newlyFreeBloc->nextBlocPtr = debutListe;

	debutListe = newlyFreeBloc;	
}
