/******************/
/* exemple_ftok.c */
/******************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>

int main() {
  system("touch fich1; touch fich2; ls -i fich*");
  printf("Cle fich1/ projet 0 et 1 = %d, %d\n",
	 (int) ftok("fich1", 0), (int) ftok("fich1", 1));
  printf("Cle fich2/ projet 0 et 1 = %d, %d\n",
	 (int) ftok("fich2", 0), (int) ftok("fich2", 1));
  /* ecrasement de fich1 par fich2 */
  system("mv fich2 fich1; ls -i fich*");
  printf("Cle fich1/ projet 0 et 1 = %d, %d\n",
	 (int) ftok("fich1", 0), (int) ftok("fich1", 1));
  system("rm fich1; ls -i fich*");
  return EXIT_SUCCESS;
}
