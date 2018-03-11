/*************************/
/* exemple_shm.c */
/*************************/

/* Processus permettant de gerer un pile en memoire partagee
   Appel:
      exemple_shm c        # Cree la memoire partage
      exemple_shm s        # Supprime la memoire partagee
      exemple_shm u valeur # pUsh valeur dans la pile
      exemple_shm o        # pOp une valeur de la pile
   Resultat : Message indiquant le resultat de l'operation
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/* Cle d'acces a la memoire partagee */
#define SHMKEY 30       

/* Nombre maximum d'elements dans la pile */
#define MAX_ELEMENTS 3

/* Structure contenant la pile */
typedef struct {
  int contenuPile[MAX_ELEMENTS];
  int nbElements;
} Pile;

int main(int argc, char *argv[]) {
  int shm_id;
  Pile *pile;
  int valeur;

  if (argc < 2){
    fputs( "USAGE = exemple_shm c        # Cree la memoire partage\n\
\texemple_shm s        # Supprime la memoire partagee\n\
\texemple_shm u valeur # pUsh valeur dans la pile\n\
\texemple_shm o        # pOp une valeur de la pile\n", stderr);
    exit(EXIT_FAILURE);
  }

  switch (argv[1][0]) {

  case 'c':
    /***************************************************************/
    /* Creation (et initialisation) d'une zone de memoire partagee */
    /***************************************************************/

    /*creation segment partage*/
    if ((shm_id = shmget(SHMKEY, sizeof(Pile), 0700|IPC_CREAT)) < 0) {
      perror("shmget pour creer la pile");
      exit(EXIT_FAILURE);
    }
    /* attachement */
    if ((pile = (Pile *)shmat(shm_id, NULL, 0)) < (Pile *) 0) {
      perror("shmat");
      exit(EXIT_FAILURE);
    }
    /* Initialisation de la structure de donnee */
    pile->nbElements = 0;
    /* Detachement */
    if (shmdt(pile)<0) {
      perror("shmdt");
      exit(EXIT_FAILURE);
    } 
    /* Affichage du resultat */
    printf("Pile cree et initialisee\n");
    break;

  case 's':
    /**********************************************/
    /* Suppression de la zone de memoire partagee */
    /**********************************************/

    /* Recherche segment partage */
    if ((shm_id = shmget(SHMKEY, sizeof(Pile), 0)) < 0) {
      perror("shmget pour supprimer la pile");
      exit(EXIT_FAILURE);
    }    
    /* Suppression */
    if (shmctl(shm_id, IPC_RMID, NULL) < 0) {
      perror("shmctl");
      exit(EXIT_FAILURE);
    }
    /* Affichage du resultat */
    printf("Pile supprimee\n");
    break;

  case 'u':
    /***************************/
    /* Empilement d'une valeur */
    /***************************/
    
    /* Verification de la presence de la valeur a ajouter */
    if (argc < 3){
    fputs( "USAGE = exemple_shm c        # Cree la memoire partage\n\
\texemple_shm s        # Supprime la memoire partagee\n\
\texemple_shm u valeur # pUsh valeur dans la pile\n\
\texemple_shm o        # pOp une valeur de la pile\n", stderr);
      exit(EXIT_FAILURE);
    }
    /* Recherche segment partage */
    if ((shm_id = shmget(SHMKEY, sizeof(Pile), 0)) < 0) {
      perror("shmget pour empiler valeur");
      exit(EXIT_FAILURE);
    }
    /* attachement */
    if ((pile = (Pile *)shmat(shm_id, NULL, 0)) < (Pile *) 0) {
      perror("Serveur : shmat");
      exit(EXIT_FAILURE);
    }
    /* La pile est-elle pleine ? */
    if (pile->nbElements >= MAX_ELEMENTS) {
      fputs("Trop d'elements dans la pile pour pouvoir en ajouter un autre\n", stderr);
      exit(EXIT_FAILURE);
    }
    /* On empile l'element */
    pile->contenuPile[pile->nbElements] = atoi(argv[2]);
    pile->nbElements += 1;
    /* Detachement */
    if (shmdt(pile)<0) {
      perror("shmdt");
      exit(EXIT_FAILURE);
    } 
    /* Affichage du resultat */
    printf("Valeur '%d' empilee dans la pile\n", atoi(argv[2]));
    break;

  case 'o':
    /***************************/
    /* Depilement d'une valeur */
    /***************************/
    
    /* Recherche segment partage */
    if ((shm_id = shmget(SHMKEY, sizeof(Pile), 0)) < 0) {
      perror("shmget pour depiler valeur");
      exit(EXIT_FAILURE);
    }
    /* attachement */
    if ((pile = (Pile *)shmat(shm_id, NULL, 0)) < (Pile *) 0) {
      perror("Serveur : shmat");
      exit(EXIT_FAILURE);
    }
    /* La pile est-elle vide ? */
    if (pile->nbElements <= 0) {
      fputs("Aucun element dans la pile ==> Impossible d'en depiler un\n", stderr);
      exit(EXIT_FAILURE);
    }
    /* On depile l'element */
    pile->nbElements -= 1;
    valeur = pile->contenuPile[pile->nbElements];
    /* Detachement */
    if (shmdt(pile)<0) {
      perror("shmdt");
      exit(EXIT_FAILURE);
    } 
    /* Affichage du resultat */
    printf("Valeur '%d' depilee de la pile\n", valeur);
    break;

  default:
    fprintf(stderr, "Argument '%s' inconnu\n", argv[1]);
    fputs( "USAGE = exemple_shm c        # Cree la memoire partage\n\
\texemple_shm s        # Supprime la memoire partagee\n\
\texemple_shm u valeur # pUsh valeur dans la pile\n\
\texemple_shm o        # pOp une valeur de la pile\n", stderr);
      exit(EXIT_FAILURE);

  }

  return EXIT_SUCCESS;
}
