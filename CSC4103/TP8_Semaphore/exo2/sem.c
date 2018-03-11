#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>
#include <semaphore.h>  
#include <fcntl.h>  
#include <sys/stat.h> 
#include <sys/wait.h>

int main (int argc, char* argv[])
{
	if(argc == 1) {
		printf("\nCe programme demande un entier en argumant pour fonctionner\n\n");
		return EXIT_FAILURE;
	}


	sem_t* sem;
	int nbClientsMax = 1;
	char* ficSync = "ficSync";

	int n = atoi(argv[1]);
	printf("\nLa variable n vaut %d\n", n);

	sem = sem_open(ficSync, O_CREAT, S_IRWXU, nbClientsMax);
	if (sem == SEM_FAILED) {  
		perror("sem_open␣failed");  
		exit(EXIT_FAILURE);  
	} 
	
	sem_unlink(ficSync);


	for(int i=0; i<n; i++) {
		fork();

		sem_wait(sem);
		printf("\n[%d] start job with courage\n", getpid());
		sleep(1);
		printf("[%d] failed miserably to do what it had to\n", getpid());
		sem_post(sem);
	}
	
	return	EXIT_SUCCESS;
}



