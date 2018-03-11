#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#include <pthread.h>

/* This program requires HWLoc.
 * On most systems, you need to install the libhwloc-dev or hwloc-devel package
 */

#define NB_TH 2
#define NB_ITER 100000000

#define GET_TIME(t) gettimeofday(&(t), NULL)
#define TIME_DIFF(t1, t2) (((t2).tv_sec - (t1).tv_sec)*1e6 + ((t2).tv_usec - (t1).tv_usec))

pthread_barrier_t barrier;

// size (in byte) of a cache line
#define CACHE_LINE_BITS 6
#define CACHE_LINE_SIZE 2<<CACHE_LINE_BITS
// number of bits of the index field
#define INDEX_BITS 9
#define INDEX_SIZE 1<<INDEX_BITS

// size (in byte) of the cache
#define CACHE_SIZE ((size_t)512*1024)

#define NB_WAY 8

#define NB_COL (1<<(INDEX_BITS+ CACHE_LINE_BITS-1))
#define NB_LIGNES (CACHE_SIZE)/NB_COL

uint8_t array[NB_LIGNES][NB_COL];

void print_bits(void* addr) {
  int i;
  printf("%p =>", addr);
  uintptr_t ptr = (uintptr_t) addr;
  int line_separator = CACHE_LINE_BITS;
  int index_separator = INDEX_BITS + line_separator;

  for(i=(sizeof(void*)*8)-1; i>=0; i--) {
    unsigned plop = 1 &ptr>>i ;
    printf("%d", plop);
    if(i == line_separator || i == index_separator)
      printf(" ");
  }
  printf("\n");
}

void *thread_func(void*arg) {
  int* counter = (int*)arg;
  int ret;
  ret = pthread_barrier_wait(&barrier);
  assert(ret == 0 || ret == PTHREAD_BARRIER_SERIAL_THREAD);
  int i;

  for(i=0; i<NB_ITER; i++) {
    (*counter) = i;
  }

  ret = pthread_barrier_wait(&barrier);
  assert(ret == 0 || ret == PTHREAD_BARRIER_SERIAL_THREAD);

  printf("result[%p]=%d\n", counter, *counter);
  return NULL;
}

int main(int argc, char**argv) {
  int ret;
  int i, j;
  pthread_t tids[NB_TH];

  int counters[NB_TH];
  struct timeval t1, t2;

  printf("nb_col=%d\n", NB_COL);
  printf("nb_lignes=%ld\n", NB_LIGNES);
  printf("index size: %d\n", INDEX_SIZE);
  printf("cache line size: %d\n", CACHE_LINE_SIZE);
  printf("-> %d bytes\n", INDEX_SIZE*CACHE_LINE_SIZE);
  printf("dec: %d bytes\n", 2<<(INDEX_BITS+CACHE_LINE_BITS));

  
  //  return 0;
  GET_TIME(t1);

  for(i=0; i<NB_ITER; i++) {
    if(i<5)
      printf("Iter %d\n", i);

    for(j=0; j<NB_WAY*2; j++) {
#if 0
      if(i<5) {
	printf("addr[%d] = ", j);
	//	print_bits(&array[j][i%NB_COL]);
		print_bits(&array[j][0]);
      }
#endif
      // array[j][i%NB_COL] = i;
      array[j][0] = i;
    }
  }
  GET_TIME(t2);

  print_bits(&array[0][0]);
  double duration = TIME_DIFF(t1, t2)/1000;
  printf("%d iterations in %lf ms\n", NB_ITER, duration);
  return EXIT_SUCCESS;
}
