#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


pid_t getpid(void);

int main(int argc, char *argv[])
{
    srand(getpid());
    int valeur_seek = rand() % 100;
    int x = -1;
    
    /* printf("%d", valeur_seek); */

    for (int i = 0 ; i < 10 ; i++)
    {
	    scanf ("%d", &x);
	    
	    if (x > valeur_seek)
	    {
		    printf("plus petit");
	    }
	    else if (x < valeur_seek)
	    {
		    printf("plus grand");
	    }
	    else
	    {
		    printf("gagné");
		    return EXIT_SUCCESS;
	    };

    };

    return EXIT_SUCCESS;
}

