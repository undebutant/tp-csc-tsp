/**************************/
/* exempleErroc_at_line.c */
/**************************/

/* Ce programme a pour objectif d'illustrer le role de la fonction */
/* error_at_line qui est *specifique* a gcc                        */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <errno.h>
#include <error.h>

#define NOM_FICHIER "unFichierQuiNExistePas"

int main(){
  struct stat buf;
  int rc;

  rc = stat(NOM_FICHIER, &buf);
  if (rc < 0){
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
		  "Pb au moment de l'appel a stat sur fichier \"%s\"",
		  NOM_FICHIER);
  }

  return EXIT_SUCCESS;
}
