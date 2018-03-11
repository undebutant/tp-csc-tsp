#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "student.h"



void generate (FILE* inputFile, FILE* outputFile, int noteIni) {
	char* resultGet = NULL;
	char buffer[NB_CHAR_READ];
	struct student eleve;

	do {
		resultGet = fgets(buffer, NB_CHAR_READ, inputFile);
		if (resultGet != NULL) {
			buffer[strlen(buffer) - 1] = 0;
			
			memcpy(eleve.name, buffer, NB_CHAR_READ);
			eleve.rank = noteIni;

			fwrite(&eleve, sizeof(struct student), 1, outputFile);
		}
		
	} while (resultGet != NULL);

	printf("Fin de la fonction generate.\n\n");
}


int main (int argc, char* argv[])
{
	if (argc < 3) {
		fprintf(stderr, "Il manque des paramètres pour l'exécution du programme.");
		return EXIT_FAILURE;
	}

	int noteEleve = atoi(argv[3]);


        FILE* inputFile = fopen(argv[1], "r");
        if (errno != 0) {
                fprintf(stderr, "Problème lors de l'ouverture du fichier input : %s\n\n", strerror(errno));
	        return EXIT_FAILURE;
	}

	FILE* outputFile = fopen(argv[2], "w");
	if (errno != 0) {
		fprintf(stderr, "Problème lors de l'ouverture du fichier output : %s\n\n", strerror(errno));    
		return EXIT_FAILURE;
	}
				        
	printf("\nDébut de la fonction generate...\n");	
	generate(inputFile, outputFile, noteEleve);

	
	int codeFermetureInput = fclose(inputFile);
	if (codeFermetureInput != 0) {
		fprintf(stderr, "Problème lors de la fermeture du fichier input : %d\n\n", codeFermetureInput); 
		return EXIT_FAILURE;
	}

	int codeFermetureOutput = fclose(outputFile);
	if (codeFermetureOutput != 0) {
	        fprintf(stderr, "Problème lors de la fermeture du fichier output : %d\n\n", codeFermetureOutput);
	        return EXIT_FAILURE;
	}
				        
	return  EXIT_SUCCESS;	

}

