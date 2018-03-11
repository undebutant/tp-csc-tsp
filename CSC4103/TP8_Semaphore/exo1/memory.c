#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>
#include <sys/wait.h>

int var = 42;

int main (int argc, char* argv[])
{
	int ret_val = fork();
	char* nomFicSync = "sync";
	FILE* fichierSync = fopen(nomFicSync, "r");
	
	// Père
	if (ret_val != 0) {
		printf("\nMoi, le grand PID %d, je suis ton père !\n", getpid());

		while(!fichierSync) {
			sleep(1);
			fichierSync = fopen(nomFicSync, "r");
		}

		fclose(fichierSync);
		remove(nomFicSync);
		printf("\n\nBloc B2\n\n");

		printf("Le père de PID %d possède un var valant %d à l'adresse %p\n", getpid(), var, &var);
	}
	// Fils
	else {
		printf("Impossibru, aussi vrai que j'ai le PID %d !\n", getpid());
		
		printf("\n\nBloc B1\n\n");

		var = 666;
		printf("Le fils de PID %d possède un var valant %d à l'adresse %p\n", getpid(), var, &var);

		fopen(nomFicSync, "a");
	}
	
	return	EXIT_SUCCESS;
}



