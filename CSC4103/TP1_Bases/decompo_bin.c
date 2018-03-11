#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void int_to_bits (int x) {

//	if (x & 8) {
//		printf ("1");
//	}
//	else {
//		printf ("0");
//	}
//
//	
//	if (x & 4) {
//		printf ("1");
//	}
//	else {
//		printf ("0");
//	}
//
//	if (x & 2) {
//		printf ("1");
//	}
//	else {
//		printf ("0");
//	}
//
//	if (x & 1) {
//		printf ("1");
//	}
//	else {
//		printf ("0");
//	}

	if (x == 1) {
		printf("1");
	}
	else if (x == 0) {
		printf("0");
	}
	else
	{
		int_to_bits (x >> 1);
		
		if (x & 1) {
			printf("1");
		}
		else {
			printf("0");
		}
	}

}


int main(int argc, char *argv[])
{
	
	int x;

	scanf("%d", &x);

	int_to_bits (x);
		
	return EXIT_SUCCESS;
}

