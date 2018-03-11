#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int facto (int n) {
	
	if (n == 0 || n == 1)
	{
		return 1;
	}
	else
	{
		for ( int mark = n - 1 ; mark > 1 ; mark --)
		{
			n *= mark;
		}
	}

	return n;
}


double power (double x, int y) {
	
	int fixe = x;

	if ( y == 0 )
	{
		return 1;
	}
	else if ( y == 1 )
	{
		return x;
	}
	else
	{
		for (int i = y - 1 ; i > 0 ; i--)
		{
			x *= fixe;
		}
	}

	return x;
}


double expo (double x, int y) {

	double exp = 0;

	for (int i = 0 ; i < y ; i++)
	{
		printf("valeur du power %lf \n", power(x,i));
		printf("valeur du facto %d \n", facto(i));
		
		exp += power(x,i)/facto(i);
	}

	return exp;
}


int main(int argc, char *argv[])
{
	
	double x;
	int y;

	scanf("%lf", &x);
	scanf("%d", &y);

	printf ("résultat : %lf",expo (x,y));

	
	
	return EXIT_SUCCESS;
}

