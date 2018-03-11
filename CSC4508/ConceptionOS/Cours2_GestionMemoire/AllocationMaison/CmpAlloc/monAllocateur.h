/* Allocateur base sur la constitution d'une liste chaine de bloc pointee */
/* par debutListe                                                         */

/* Taille des blocs sur lesquels se font les experiences. */
#define TAILLEBLOC 16

/**************************************************************************/
/* Routine d'initialisation de l'allocateur : constitue une liste chainee */
/* de nbMaxAlloc blocs de taille TAILLEBLOC, le premier bloc etant pointe */
/* par debutListe et le dernier pointant sur NULL                         */
/**************************************************************************/
void monAllocateurInit(int nbMaxAlloc);


/**************************************************************************/
/* Fonction renvoyant un pointeur sur une zone memoire                    */
/* NB : on ne peut pas preciser la taille, puisque la taille est          */
/*      predefinie                                                        */
/**************************************************************************/
void *monMalloc(void);


/**************************************************************************/
/* Fonction liberant la zone memoire pointee par ptr                      */
/**************************************************************************/
void monFree(void *ptr);
