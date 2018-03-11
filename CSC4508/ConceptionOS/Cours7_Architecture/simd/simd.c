#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <assert.h>
#include <immintrin.h>		/* for SSE/AVX intrinsics */

#define GET_TIME(t) gettimeofday(&(t), NULL)
#define TIME_DIFF(t1, t2) (((t2).tv_sec - (t1).tv_sec)*1e6 + ((t2).tv_usec - (t1).tv_usec))
#define NB_ITER 16*1000

void init_array(float* array, int size) {
  int i;
  for(i=0; i<size; i++){
    array[i]= i;
  }
}

void use_array(float* array, int size){
  printf("array[%d] = %f\n", size/2, array[size/2]);
}

void compute(float* A, float*B, float*C, int size) {
  int i, n;
  for(n=0; n<NB_ITER; n++) {
    for(i=0; i<size; i++) {
      C[i] = A[i] * B[i];
    }
  }
}

#if HAVE_SSE
void compute_SSE(float* A, float*B, float*C, int size) {
  __m128* pA = NULL;
  __m128* pB = NULL;
  __m128* pC = NULL;
  int i, n;

  for(n=0; n<NB_ITER; n++) {
    pA = (__m128*) A;
    pB = (__m128*) B;
    pC = (__m128*) C;

    for(i=0; i<size; i+=4) {
      /* computes ranks i, i+1, i+2, i+3 */
      *pC = _mm_mul_ps(*pA, *pB);
      pA++;
      pB++;
      pC++;
    }
  }
}
#endif	/* HAVE_SSE */

#if HAVE_AVX
void compute_AVX(float* A, float*B, float*C, int size) {
  __m256* pA = NULL;
  __m256* pB = NULL;
  __m256* pC = NULL;
  int i, n;

  for(n=0; n<NB_ITER; n++) {
    pA = (__m256*) A;
    pB = (__m256*) B;
    pC = (__m256*) C;

    for(i=0; i<size; i+=8) {
      /* computes ranks i, i+1, .., i+7 */
      *pC = _mm256_mul_ps(*pA, *pB);
      pA++;
      pB++;
      pC++;
    }
  }
}
#endif	/* HAVE_AVX */


int main(int argc, char**argv){
  int size = 128*1024;
  float *A;
  float *B;
  float *C;
  int ret;
  ret = posix_memalign((void**)&A, 32, size*sizeof(float));
  assert(!ret);
  ret = posix_memalign((void**)&B, 32, size*sizeof(float));
  assert(!ret);
  ret = posix_memalign((void**)&C, 32, size*sizeof(float));
  assert(!ret);

  if(argc<2) {
    fprintf(stderr, "Usage: %s <0|1|2>\n", argv[0]);
    fprintf(stderr, "\t where 0 -> standard version\n");
    fprintf(stderr, "\t where 1 -> SSE version\n");
    fprintf(stderr, "\t where 2 -> AVX version\n");
    return 1;
  }
  struct timeval t1, t2;

  init_array(A, size);
  init_array(B, size);

  GET_TIME(t1);
  if(strcmp(argv[1], "0")==0) {
    compute(A, B, C, size);
  } else if(strcmp(argv[1], "1")==0) {
#if HAVE_SSE
    compute_SSE(A, B, C, size);
#else
    fprintf(stderr, "SSE not available on this system\n");
    return EXIT_FAILURE;
#endif
  } else if(strcmp(argv[1], "2")==0) {
#if HAVE_AVX
    compute_AVX(A, B, C, size);
#else
    fprintf(stderr, "AVX not available on this system\n");
    return EXIT_FAILURE;
#endif
  }

  GET_TIME(t2);
  use_array(C, size);
  printf("%d items computed in %lf ms\n", size*NB_ITER, TIME_DIFF(t1, t2)/1000);
  return EXIT_SUCCESS;
}
