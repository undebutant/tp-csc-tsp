/*************************************************************
 * Module destiné à simplifier l'utilisation de getrusage    *
 * Utilisation typique :                                     *
 *    - getrusageStart() pour démarrer la mesure             *
 *    - getRusageStop() pour arrêter la mesure et            *
 *      afficher les résultats de la mesure                  *
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
void getrusageStart();

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
void getrusageStop();
