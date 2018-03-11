#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>



// Pour stdin et stdout, c'est des flux, donc juste remplacer le inpuFile et outputFile, mais voir problème pour arrêter capture

void encode (uint8_t convertKey, FILE* inputFile, FILE* outputFile) {
	int endOfFile = 0;
	char buffer[256];
	char encoded[256];
	char caracBuff;
	char caracEncoded;

	do {
		endOfFile = fread(buffer, sizeof(char), 256, inputFile);
		if (errno != 0) {
			fprintf(stderr, "Erreur sur la lecture : %s\n\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		if (endOfFile != 0) {
			for (int i = 0 ; i < endOfFile ; i++) {
				caracBuff = buffer[i];
				caracEncoded = caracBuff ^ convertKey;
				encoded[i] = caracEncoded;
			}

			fwrite(&encoded, sizeof(char), endOfFile, outputFile);
			if (errno != 0) {
				fprintf(stderr, "Erreur sur l'écriture : %s\n\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
		}

	} while (endOfFile != 0);
}


int main (int argc, char* argv[])
{
	if (argc < 3) {
		fprintf(stderr, "Il manque des paramètres pour l'exécution du programme.");
		return EXIT_FAILURE;
	}
	

	uint8_t convertKey = (uint8_t) atoi(argv[1]);
	fprintf(stderr, "\nEncodage du fichier %s à partir de la clé %d\n\n", argv[2], convertKey);
	
	FILE* inputFile = fopen(argv[2], "r");
	if (errno != 0) {
		fprintf(stderr, "Problème lors de l'ouverture du fichier input : %s\n\n", strerror(errno));
		return EXIT_FAILURE;
	}


	FILE* outputFile = fopen(argv[3], "w");
	if (errno != 0) {
		fprintf(stderr, "Problème lors de l'ouverture du fichier output : %s\n\n", strerror(errno));	
		return EXIT_FAILURE;
	}
	

	fprintf(stderr, "\nDémarrage de la fonction encode...\n\n");
	encode(convertKey, inputFile, outputFile);
	fprintf(stderr, "Fin de la fonction encode.\n\n");


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
	
	return	EXIT_SUCCESS;
}



