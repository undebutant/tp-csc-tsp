#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "outils.h"

static size_t NBMEG = 3;
#define NB_ITER 10000

#define BYTES (NBMEG<<20)
#define PAGE 4096
#define NBRE (BYTES/PAGE)
#define TIME_DIFF(t1, t2) ((t2.tv_sec-t1.tv_sec)*1e6+(t2.tv_usec-t1.tv_usec))

char **t;

int main(int argc, char** argv) {
  int iter, x, y;
  unsigned long s=0;
  struct timeval t1, t2;
  srand(time(NULL));

  /* allocation du tableau t */
  if(argc > 1) {
    NBMEG = atoi(argv[1]);
  }
  t = allocate_memory(NBRE, PAGE);
  if(!t)
    return EXIT_FAILURE;

  /* On attache le thread courant a un coeur (par defaut, le coeur #0) */
  int core_num = 0;
  if(argc > 2) {
    core_num = atoi(argv[2]);
  }
  printf("Binding current thread to core #%d\n", core_num);
  if(bind_to_core(core_num)) {
    fprintf(stderr, "Warning: cannot bind to core %d\n", core_num);
  }

  /* debut de la mesure */
  printf("START !\n");

  if(gettimeofday(&t1, NULL)) {
    perror("gettimeofday");
  }

  for(iter=0; iter < NB_ITER; iter ++) {
    /* acces a une case du tableau au hasard */
    x = rand()%NBRE;
    y = rand()%PAGE;
    t[x][y] = x*y;
    s += t[x][y];
  }

  /* fin de la mesure */
  if(gettimeofday(&t2, NULL)) {
    perror("gettimeofday");
  }
  printf("STOP !\n");

  /* affichage du resultat */
  printf("%d iterations in %lf s (%lf µs per iteration)\n",
	 NB_ITER, TIME_DIFF(t1, t2)/1e6, TIME_DIFF(t1, t2)/NB_ITER);

  return EXIT_SUCCESS;
}
