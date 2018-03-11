#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <signal.h>  
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>



void add_pid (const char* path) {
	int pidActuel = getpid();

	FILE* filePID = fopen(path, "a");

	printf("\nOn ecrit le PID %d dans le file\n\n", getpid());
	fwrite(&pidActuel, sizeof(pidActuel), 1, filePID);

	fclose(filePID);
}


pid_t* get_pids(const char* path, int* n) {
	FILE* filePID = fopen(path, "r");
	fseek(filePID, 0, SEEK_END);

	int sizePID = sizeof(getpid());
	int nbPIDs = ftell(filePID) / sizePID;
	*n = nbPIDs;
	fseek(filePID, 0, SEEK_SET);
	
	pid_t* result = malloc(sizePID * nbPIDs);
	fread(result, sizeof(sizePID), nbPIDs, filePID);

	fclose(filePID);

	return result; 
}


void list_pids (const char* path) {
	int nbPIDs = 0;
	pid_t* result = get_pids(path, &nbPIDs);

	for(int i = 0 ; i < nbPIDs ; i++) {
		printf("PIDs : %d\n", result[i]);
	}
	 
	free (result);
}


int self_pid(pid_t* pids, int n) {
	int currentPID = getpid();

	for(int i = 0 ; i < n ; i++) {
		if(pids[i] == currentPID) {
			printf("\nLe PID cherché se trouve à l'indice %d\n", i);
			free(pids);
			return i;
		}
	}
	printf("\nErreur : PID introuvable\n");
	free(pids);
	exit(-1);
}


void remove_pids(const char* path) {
	pid_t* result, newTab;
	int nbPIDs = 0;
	int posPID = -1;
	int fileModif;

	currentPID = getpid();
	result = get_pids(path, nbPIDs);
	posPID = self_pid(result, nbPIDs);

	result = get_pids(path, nbPIDs);
	result[posPID] = result[nbPIDs - 1];
	
	fileModif = fopen(path, "w");
	fwrite(&result, sizeof(pid_t), nbPIDs - 1, fileModif);

	fclose(fileModif);
	free(result);
}


int main (int argc, char* argv[])
{
	int nbPIDs = 0;

	if(argc < 2) {
		printf("\nIl faut un argument au moins\n\n");
		exit(0);
	}

	for(int i = 0 ; i < 5 ; i++) {
		add_pid(argv[1]);
		nbPIDs++;
		
		if(i == 2) {
			int pidToDelete = getpid // A corriger, pas plusieurs instances => PID différents si for
		}

		list_pids(argv[1]);
	
		pid_t* listPID = get_pids(argv[1], &nbPIDs);
		self_pid(listPID, nbPIDs);

	return	EXIT_SUCCESS;
}



