#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>
#include <sys/wait.h>



int main (int argc, char* argv[])
{
	int fork_nb = 0;

	printf("The current process has PID %d, et est le fils de %d\n", getpid(), getppid());
	
	while(fork_nb < 3) {
		if(fork() != 0) {
			break;
		}
		else {
			printf("The current process has PID %d, et est le fils de %d\n", getpid(), getppid());
			fork_nb ++;
		}
	}
	
	if(fork_nb >= 1) {
		if(fork_nb == 3) {
			//char* cmd_to_exec = "ps f";
			//system(cmd_to_exec);
			execlp("ps", "ps", "-l", NULL);
		}
		else {
			sleep(3);
		}
	}
	
	int status;
	pid_t pid = wait(&status);
	int exit_status = WEXITSTATUS(status);  
	printf("Process %d had the child process %d ended with exit status %d\n", getpid(), pid, exit_status);

	return	EXIT_SUCCESS;
}



