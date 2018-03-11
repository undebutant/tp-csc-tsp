/**************************/
/* exempleERROR_AT_LINE.c */
/**************************/

/* Ce programme a pour objectif d'illustrer le role de la macro ERROR_AT_LINE */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
   #define ERROR_AT_LINE(status,errnum,filename,linenum,...) {	\
      fprintf(stderr,"%s:%d:", filename,linenum);  \
      fprintf(stderr,__VA_ARGS__);  \
      fprintf(stderr,":%s\n", strerror(errnum));\
      abort();                             \
   }

#define NOM_FICHIER "unFichierQuiNExistePas"

int main(){
  struct stat buf;
  int rc;

  rc = stat(NOM_FICHIER, &buf);
  if (rc < 0){
    ERROR_AT_LINE(EXIT_FAILURE, errno, __FILE__, __LINE__,
		  "Pb au moment de l'appel a stat sur fichier \"%s\"",
		  NOM_FICHIER);
  }

  return EXIT_SUCCESS;
}
