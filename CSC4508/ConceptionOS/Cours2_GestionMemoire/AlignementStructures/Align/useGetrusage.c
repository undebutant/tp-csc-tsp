/*************************************************************
 * Module destiné à simplifier l'utilisation de getrusage    *
 * Utilisation typique :                                     *
 *    - getrusageStart() pour démarrer la mesure             *
 *    - getRusageStop() pour arrêter la mesure et            *
 *      afficher les résultats de la mesure                  *
 *************************************************************/

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "useGetrusage.h"


/*************************************************************
 * Variables internes                                        *
 *************************************************************/

/* Variable de stockage de l'appel de getrusage au moment du start()
*/
static struct rusage startRusage;


/*************************************************************
 * APIs                                                      *
 *************************************************************/

/* API getrusageStart()
 *
 * Description
 *   Cette procedure, appelee au debut d'une seance de mesure, permet
 *   de memoriser le resultat de l'execution de getrusage.
 *
 * Valeur renvoyee
 *   Sans objet
 */
void getrusageStart() {
  if (getrusage( RUSAGE_SELF, &startRusage )) {
    perror("getrusage dans start() a echoue");
    exit(1);
  }
}

/* API getrusageStop()
 *
 * Description
 *   Cette procedure, appelee a la fin d'une seance de mesure, permet de
 *   calculer et d'afficher la variation des grandeurs suivantes (qui 
 *   sont celles remplies par Linux) entre l'appel a getrusageStart()
 *   et l'appel a getrusageStop(). Cf `man getrusage` pour le sens de
 *   ces valeurs :
 *       - ru_utime
 *       - ru_stime
 *       - ru_minflt
 *       - ru_majflt
 *       - ru_nswap 
 * Cette procedure affiche egalement la grandeur suivante :
 *       - ru_utime + ru_stime
 *
 * Valeur renvoyee
 *   Sans objet
 */
void getrusageStop() {
  struct rusage stopRusage;
  struct timeval diffTimeval;
  struct timeval startSomme;
  struct timeval stopSomme;

  if (getrusage( RUSAGE_SELF , &stopRusage ) < 0) {
    perror("getrusage dans stopAndPrintResult() a echoue");
    exit(1);
  }
  else {
    /* Debut de l'affichage des champs standards (et remplis par      */
    /* Linux, comme explique dans le `man getrusage`) d'une structure */
    /* rusage                                                         */
    /* NB: timersub et timeradd sont des macros déclarees dans        */
    /* <sys/time.h>                                                   */
    timersub(&stopRusage.ru_utime, &startRusage.ru_utime, &diffTimeval);
    (void)printf("ru_utime          = %8d.%6d s\n",
		 (int)diffTimeval.tv_sec,
		 (int)diffTimeval.tv_usec);
    timersub(&stopRusage.ru_stime, &startRusage.ru_stime, &diffTimeval);
    (void)printf("ru_stime          = %8d.%6d s\n",
		 (int)diffTimeval.tv_sec,
		 (int)diffTimeval.tv_usec);

    /* Calcul et affichage de la somme du temps user et du temps      */
    /* systeme                                                        */
    timeradd(&startRusage.ru_utime, &startRusage.ru_stime, &startSomme);
    timeradd(&stopRusage.ru_utime, &stopRusage.ru_stime, &stopSomme);
    timersub(&stopSomme, &startSomme, &diffTimeval);
    (void)printf("ru_utime+ru_stime = %8d.%6d s\n", 
		 (int)diffTimeval.tv_sec,
		 (int)diffTimeval.tv_usec);

    /* on poursuit l'affichage des champs standards (et remplis par   */
    /* Linux, comme explique dans le `man getrusage`) d'une structure */
    /* rusage                                                         */
    (void)printf("ru_minflt = %8d\n",
		 (int)(stopRusage.ru_minflt - startRusage.ru_minflt) );
    (void)printf("ru_majflt = %8d\n",
		 (int)(stopRusage.ru_majflt - startRusage.ru_majflt) );
    (void)printf("ru_nswap  = %8d\n",
		 (int)(stopRusage.ru_nswap - startRusage.ru_nswap) );
  }
}
