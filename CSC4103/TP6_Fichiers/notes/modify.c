#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "student.h"



void modify (FILE* inputFile, char* nomEleve, int noteModif) {
	long tailleFile = 0;
	int posEleveModif = -1;
	

	fseek(inputFile, 0, SEEK_END);
	tailleFile = ftell(inputFile);
	fseek(inputFile, 0, SEEK_SET);
	printf("Le fichier fait %ld octets\n", tailleFile);

	int nbStudent = tailleFile / sizeof(struct student);
	printf("Il contient %d étudiants\n\n", nbStudent);	
	struct student* tabEleve = malloc(sizeof(struct student) * nbStudent);
	

	fread(tabEleve, sizeof(struct student), nbStudent, inputFile);
	
	// Display
	for (int i = 0 ; i < nbStudent ; i++) {
		printf("%s, %d\n", tabEleve[i].name, tabEleve[i].rank);
	}
	

	// Modif
	for (int j = 0 ; j < nbStudent ; j++) {
		if (!strcmp(nomEleve, tabEleve[j].name)) {
			posEleveModif = j;
			printf("\nL'élève se troue en position %d du tableau\n", posEleveModif);
		}
	}

	fseek(inputFile, (sizeof(struct student) * posEleveModif) + (NB_CHAR_READ * sizeof(char)), SEEK_SET);
	fwrite(&noteModif, sizeof(int), 1, inputFile);
	fseek(inputFile, 0, SEEK_SET);


	printf("\n");
	
	
	fread(tabEleve, sizeof(struct student), nbStudent, inputFile);

	// Display
	for (int i = 0 ; i < nbStudent ; i++) {
		printf("%s, %d\n", tabEleve[i].name, tabEleve[i].rank);
	}

	free(tabEleve);
	printf("\nFin de la fonction modify.\n\n");
}


int main (int argc, char* argv[])
{
	if (argc < 3) {
		fprintf(stderr, "Il manque des paramètres pour l'exécution du programme.");
		return EXIT_FAILURE;
	}

	int noteEleve = atoi(argv[3]);


        FILE* inputFile = fopen(argv[1], "r+");
        if (errno != 0) {
                fprintf(stderr, "Problème lors de l'ouverture du fichier input : %s\n\n", strerror(errno));
	        return EXIT_FAILURE;
	}

	
	printf("\nDébut de la fonction modify...\n\n");	
	modify(inputFile, argv[2], noteEleve);

	
	int codeFermetureInput = fclose(inputFile);
	if (codeFermetureInput != 0) {
		fprintf(stderr, "Problème lors de la fermeture du fichier input : %d\n\n", codeFermetureInput); 
		return EXIT_FAILURE;
	}

	return  EXIT_SUCCESS;	

}

