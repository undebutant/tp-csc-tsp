#define _GNU_SOURCE             /* See feature_test_macros(7) */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>

#include "outils.h"

static size_t NBMEG = 2000;

#define BYTES (NBMEG<<20)
#define PAGE 4096
#define NBRE (BYTES/PAGE)
#define TIME_DIFF(t1, t2) ((t2.tv_sec-t1.tv_sec)*1e6+(t2.tv_usec-t1.tv_usec))

char **t;

int main(int argc, char** argv) {
  int i, j;
  uint64_t s=0;

  /* allocation du tableau t */
  if(argc > 1) {
    NBMEG = atoi(argv[1]);
  }
  t = allocate_memory(NBRE, PAGE);
  if(!t)
    return EXIT_FAILURE;

  int core_num = 1;
  /* On attache le thread courant a un coeur (par defaut, le coeur #1) */
  if(argc > 2) {
    core_num = atoi(argv[2]);
  }
  printf("Binding current thread to core #%d\n", core_num);
  if(bind_to_core(core_num)) {
    fprintf(stderr, "Warning: cannot bind to core %d\n", core_num);
  }

  while(1) {
    for(i=0; i< NBRE; i++){
      for(j=0; j<PAGE; j+=32)
	t[i][j]=s++;
    }
  }

  return EXIT_SUCCESS;
}
