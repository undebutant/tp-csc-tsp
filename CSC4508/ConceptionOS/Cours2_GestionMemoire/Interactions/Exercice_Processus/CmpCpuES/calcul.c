#include <stdlib.h>
#include <math.h>

#define NBFOIS 260

int main(){
  double s,x;
  int i,j;

  for (j=0 ; j<NBFOIS ; j++) {
    s = 0;
    for (i=0 ; i<500000 ; i++) {
      x = sin(3.141592654 * s);
      x = cos(3.141592654 * s);
      x = tan(3.141592654 * s);
      s += 0.1;
    }
  }

  s = x; // Ligne pour eviter un warning du compilateur
         // pour signaler que x n'est jamais lue             

  return EXIT_SUCCESS;
}
       
