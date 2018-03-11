// ----------------------------------------
// Tous les exemples de code suivant passent la compilation.
// Mais, ils ont un petit souci qui pourrait donner une erreur
// a l'execution.
// Trouver chacun des soucis, comme si vous faisiez une revue de code !
// ----------------------------------------

/*--- Code 01 ---*/
open("toto", O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);

// Pas de récupération du descripteur, et pas d'assert

/*--- Code 02 ---*/
fd = open("toto", O_RDWR|O_CREAT);
assert(fd >= 0);

// Pas de mode précisé alors qu'on créé un fichier

/*--- Code 03 ---*/
fd = open("toto", O_RDWR&O_SYNC);
assert(fd >= 0);

// Utilisation du & au lieu du | pour mettre plusieurs flags

/*--- Code 04 ---*/
char nom[] = {'t', 'o', 't', 'o'};
fd = open(nom, O_RDWR|O_SYNC);
assert(fd >= 0);

/* L'array ne se termine pas par \0, donc le compilateur va lire jusqu'à en trouver un */

/*--- Code 05 ---*/
char *c;
nbRead = read(fd, c, sizeof(c));
assert(nbRead >= 0);

// On fait un sizeof de pointeur, et en plus on n'initialise pas le pointeur c avec c = malloc(sizeof(char)) 

/*--- Code 06 ---*/
float f;
nbRead = read(fd, &f, sizeof(int));
assert(nbRead >= 0);

// On a un float, et sizeof int

/*--- Code 07 ---*/
float *f = malloc(sizeof(float));
assert(f);
nbRead = read(fd, f, sizeof(f));
assert(nbRead >= 0);

// sizeof de f au lieu de sizeof de *f

/*--- Code 08 ---*/
double d;
read(fd, &d, sizeof(d));

// Pas d'assert

/*--- Code 09 ---*/
char *s = "ABCD";
rc = read(fd, s, strlen(s)+1);
assert(rc >= 0);

// La partie "ABCD" va etre sur une zone read only (définit dans partie read only du segment prgm
// Donc il faudrait mettre char s[5] = ... et on pourra ECRIRE dedans

/*--- Code 10 ---*/
double *pd = malloc(sizeof(double));
assert(pd);
rc = write(fd, pd, sizeof(*pd));
assert(rc >= 0);

// *pd n'est pas initialisé, utiliser *pd = 0 ou memset(pd, 0, sizeof(double))

/*--- Code 11 ---*/
#define CHAINE "ab"
rc = write(fd, CHAINE, sizeof(CHAINE));
assert(rc >= 0);

// strlen au lieu de sizeof

/*--- Code 12 ---*/
char *s = "ABCD";
rc = write(fd, s, strlen(s));
assert(rc >= 0);

// il manque le +1 au strlen

/*--- Code 13 ---*/
int i;
for (i = 0 ; i < 10 ; i++) {
  write(fd, i, sizeof(i));
}

// Manque le assert, et on doit mettre &i au lieu de i pour donner un pointeur

/*--- Code 14 ---*/
double d;
rc = write(fd, d, sizeof(d));
assert(rc >= 0);

// &d au lieu de d

/*--- Code 15 ---*/
close(fd);

// Manque le test sur le résultat du close










