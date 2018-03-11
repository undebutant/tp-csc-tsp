#include <stdlib.h>
#include <stdio.h>

#define NB_ELEMENTS 7

int compar_short (short* a, short* b) {
	if (*a > *b) {
		return 1;
	}
	else if (*a < *b) {
		return -1;
	}
	return 0;
}

int compar_double (double* a, double* b) {
	if (*a > *b) {
		return 1;
	}
	else if (*a < *b) {
		return -1;
	}
	return 0;
}


void print_tab_short(short tab[], size_t nmemb) {
  int i;
  for (i = 0 ; i < nmemb ; i++) {
    printf("tab_short[%d] = %d\n", i, tab[i]);
  }
}

void print_tab_double(double tab[], size_t nmemb) {
  int i;
  for (i = 0 ; i < nmemb ; i++) {
    printf("tab_double[%d] = %lf\n", i, tab[i]);
  }
}

int main(){
  short tab_short[NB_ELEMENTS] = {6, 5, 4, 3, 2, 1, 0};
  double tab_double[NB_ELEMENTS] = {7.1, 6.2, 5.3, 4.4, 3.5, 2.6, 1.7};

  printf("tab_short avant tri\n");
  print_tab_short(tab_short, NB_ELEMENTS);
  
  int (*compar_short_ptr)(const void* a,const void* b) = (void*)compar_short;

  qsort(tab_short, sizeof(tab_short)/sizeof(short), sizeof(short), compar_short_ptr);

  printf("tab_short après tri\n");
  print_tab_short(tab_short, NB_ELEMENTS);

  printf("tab_double avant tri\n");
  print_tab_double(tab_double, NB_ELEMENTS);

  int (*compar_double_ptr)(const void* a,const void* b) = (void*)compar_double;

  qsort(tab_double, sizeof(tab_double)/sizeof(double), sizeof(double), compar_double_ptr);

  printf("tab_double après tri\n");
  print_tab_double(tab_double, NB_ELEMENTS);

  return EXIT_SUCCESS;
}
