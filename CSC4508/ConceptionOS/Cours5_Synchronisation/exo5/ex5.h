/* Declarations communes a imprime4c.c et imprime4s.c */

/* Taille maxi des noms de fichiers */
#define TAILLEMAX 80
#define CASEMAX 10




struct shared_memory {
	char tab[CASEMAX][TAILLEMAX];
	int debutlect;
	int finecrit;
	sem_t info_occupe;
	sem_t place_libre;
};

#define SHM_SIZE sizeof(struct shared_memory)

