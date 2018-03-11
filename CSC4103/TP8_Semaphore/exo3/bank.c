#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>
#include <semaphore.h>  
#include <fcntl.h>  
#include <sys/stat.h> 
#include <sys/wait.h>

int main (int argc, char* argv[])
{
	if(argc < 3) {
		printf("\nCe programme demande un string et un entier en argument pour fonctionner !\n\n");
		return EXIT_FAILURE;
	}
	
	
	char* account = argv[1];
	int solde = 0;
	int soldeMod = 0;
	int modification = atoi(argv[2]);
	FILE* ficAccount;

        sem_t* sem = sem_open("ficSync", O_CREAT, S_IRWXU, 1);
	if (sem == SEM_FAILED) {
		perror("sem_open␣failed");
		exit(EXIT_FAILURE);
	}


	sem_wait(sem);
	
	ficAccount = fopen(account, "r");
	if(!ficAccount) {
		printf("\nLe compte demandé n'existe pas. Création du compte...\n");
		
		ficAccount = fopen(account, "a");
		fprintf(ficAccount, "0");
		
		printf("Compte crée, solde à 0.\n\n");
		fclose(ficAccount);
	}
	else
	{
		fscanf(ficAccount, "%d", &solde);
		printf("\nLe compte demandé a été trouvé, il a un solde de %d.\n\n", solde);
		fclose(ficAccount);
	}


	printf("On effectue l'opération sur le compte...\n");
	soldeMod = solde + modification;
	printf("Le nouveau solde de %s sera à %d.\n", account, soldeMod);	
	
	ficAccount = fopen(account, "w");
	fprintf(ficAccount, "%d", soldeMod);
	fclose(ficAccount);

	ficAccount = fopen(account, "r");
	fscanf(ficAccount, "%d", &solde);
	printf("\nAprès avoir effectué l'opération, le solde du compte est maintenant de %d.\n\n", solde);
	fclose(ficAccount);

	sem_post(sem);


	return	EXIT_SUCCESS;
}



