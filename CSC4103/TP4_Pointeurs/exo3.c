#include <stdio.h>
#include <stdlib.h>



int main (int argc, char* argv[])
{
	int Sum = 0;

	for (int i = 0 ; i < argc ; i++) {
		printf("L'argument n°%d est %s à l'adresse mémoire %p\n", i, argv[i], argv[i]);
		
		Sum += atoi(argv[i]);
	}

	printf("La somme des args vaut %d", Sum);
	
}



