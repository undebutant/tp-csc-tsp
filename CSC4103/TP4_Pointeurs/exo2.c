#include <stdio.h>
#include <stdlib.h>



void display (int* tab, int sizeTab)
{
	printf("\n\n{ ");
	for (int i = 0 ; i < sizeTab ; i++) {
		printf("%d", tab[i]);

		if (i == sizeTab - 1) {
			printf(" ");
		}
		else {
			printf(", ");
		}

	}
	printf("}\n\n");	
}


void squares (int* tab, int sizeTab)
{
	for (int i = 0 ; i < sizeTab ; i++) {
		tab[i] = tab[i] * tab[i];
	}
}


void init (int* tab, int sizeTab)
{
	for (int i = 0 ; i < sizeTab ; i++) {
		*tab = 0;
		tab++;
	}
}


int main (int argc, char* argv[])
{
	
	int tab1[4] = {1 ,2 ,3 ,4 };
	
	int* tab2 = malloc(sizeof(int)*4) ;
		
	//init (tab1, 4);

	//display(tab1, 4);
	//squares(tab1, 4);
	//display(tab1, 4);	
	
	//display(tab2, 4);
	
	printf("\nval tab1 %p\n", tab1);
	printf("val &tab1 %p\n", &tab1); 		
	printf("val tab2 %p\n", tab2);
	printf("val &tab2 %p\n\n", &tab2);

	printf("size tab1 %lu\n", sizeof(tab1));
	printf("size &tab1 %lu\n", sizeof(&tab1));
	printf("size *&tab1 %lu\n", sizeof(*&tab1));
	printf("size tab1 %lu\n", sizeof(tab2));
       	printf("size &tab1 %lu\n", sizeof(&tab2));
	printf("size *&tab1 %lu\n\n", sizeof(*&tab2));
	
	free(tab2);
	
}



