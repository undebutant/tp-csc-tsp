/*******************/
/* exempleAssert.c */
/*******************/

/* Ce programme a pour objectif d'illustrer le role de la macro assert */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <assert.h>

int main(){
  struct stat buf;
  int rc;

  rc = stat("unFichierQuiNExistePas", &buf);
  assert(rc >= 0);

  return EXIT_SUCCESS;
}
