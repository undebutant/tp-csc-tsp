#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 8192

void init(double**mat) {
  int i, j;
  for(i=0; i<N; i++) {
    for(j=0; j<N; j++) {
      mat[i][j] = i+j;
    }
  }
}

int main(){
  int i, j;
  double ** mat =malloc(sizeof(double*)*N);
  for(i=0; i<N; i++) {
    mat[i] = malloc(sizeof(double)*N);
  }
  init(mat);

  double res = 0;
  for(i=0; i<N; i++) {
    for(j=0; j<N; j++) {
      res += mat[i][j];
    }
  }
  printf("result: %lf\n", res);
  return 0;
}
