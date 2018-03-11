#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <signal.h>  
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>



unsigned long long k = 0;
unsigned long long ref = 0;
int N = 100;
int volatile n = 0; // Necessaire car sinon, avec -O3 (Oh et trois) le prgm sera compile de maniere opti
		    // Donc gcc ne voyant rien dans le main changeant n, changera while (n < N) en while(1)


void hello (int signo) {
	if(n % 1000 == 0) {
		printf("\nHello signal %d\n", signo);
	}
	k--;
	n++;
}


int main (int argc, char* argv[])
{
	int ret_val;
	struct sigaction s;
	sigset_t sBlock;
	int signo = SIGALRM;

	s.sa_handler = hello;
	s.sa_flags = 0;

	sigemptyset(&sBlock);
	sigaddset(&sBlock, SIGALRM);

	ret_val = sigaction (signo, &s, NULL);
	if(ret_val<0) {
		perror("sigaction␣failed");
		abort();
	}
	
	
	ualarm(1000, 1000);
	while(n < N) {

		sigprocmask(SIG_BLOCK, &sBlock, NULL); // cf man pthread_sigmask
		k++;
		sigprocmask(SIG_UNBLOCK, &sBlock, NULL);
		
		ref++;
	}

	printf("\nSortie de la boucle\n");

	printf("\nLa difference donne %llu\n\n", ref - k);

	return	EXIT_SUCCESS;
}



