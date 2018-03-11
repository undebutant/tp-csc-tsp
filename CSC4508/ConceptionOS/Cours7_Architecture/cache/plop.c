#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>


#define GET_TIME(t) gettimeofday(&(t), NULL)
#define TIME_DIFF(t1, t2) (((t2).tv_sec - (t1).tv_sec)*1e6 + ((t2).tv_usec - (t1).tv_usec))


#if 1
#define PAD(n) //long pad##n [256]
#else
#define PAD(n) long pad##n [256]
#endif


// size (in byte) of a cache line
#define CACHE_LINE_BITS 6
#define CACHE_LINE_SIZE 2<<CACHE_LINE_BITS
// number of bits of the index field
#define INDEX_BITS 9
#define INDEX_SIZE 1<<INDEX_BITS

// size (in byte) of the cache
#define CACHE_SIZE ((size_t)512*1024)

#define NB_WAY 8


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

long* get_buffer_start(long*buffer, int index, long max) {
#if 0
  return &buffer[index*(max+64)];
#else
  return &buffer[index*(max)];
#endif
}

void fill_buffer(long*buffer, long max) {
  int i;
  for(i=0; i<max; i++) {
    //    buffer[i] = rand()%256;
    buffer[i] = rand()%32;
  }
}
int main() {
  int n_iter =100000;
  long max = 32*1024/sizeof(long);

  long buffer[max*10];
  printf("max=%d\n", max*sizeof(long));
  printf("buffer size: %d\n", max*9*sizeof(long));
  fill_buffer(buffer, max);
  long *A = get_buffer_start(buffer, 0, max);
  long *B = get_buffer_start(buffer, 1, max);
  long *C = get_buffer_start(buffer, 2, max);
  long *D = get_buffer_start(buffer, 3, max);
  long *E = get_buffer_start(buffer, 4, max);
  long *F = get_buffer_start(buffer, 5, max);
  long *G = get_buffer_start(buffer, 6, max);
  long *H = get_buffer_start(buffer, 7, max);
  long *I = get_buffer_start(buffer, 8, max);


  int i, j;
  struct timeval t1, t2;
  GET_TIME(t1);
  int next_index = 17;
  for(j = 0; j < n_iter; j++) {
    for(i = 0; i < max; i++) {
      int index = next_index;
      A[index] = B[index]*C[index] + D[index]*E[index] + F[index]*G[index] + H[index]*I[index];
      next_index = A[index];
    }
  }
  GET_TIME(t2);

  double duration = TIME_DIFF(t1, t2)/1000;
  printf("%d iterations in %lf ms\n", n_iter, duration);


  print_bits(&A[0]);
  print_bits(&B[0]);
  print_bits(&C[0]);
  print_bits(&D[0]);
  print_bits(&E[0]);
  print_bits(&F[0]);
  print_bits(&G[0]);
  print_bits(&H[0]);
  print_bits(&I[0]);
  return 0;
}
