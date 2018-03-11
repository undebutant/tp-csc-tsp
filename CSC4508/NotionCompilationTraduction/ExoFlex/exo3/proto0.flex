%option nounput noinput

%{
#include "proto-color.h"

int mot = 0;
int motV = 0;
int ponct = 0;
int ligne = 0;
int space = 0;

%}

MOTV		(v|V)[a-zA-Zéèàœ]*
MOT		[a-zA-Zéèàœ]*
PONCTU		(\.|\,|\:|\!|\?|\'|\-)
SPACE		(\ )

%%
{MOTV}		motV++;mot++;
{MOT}		mot++;
{PONCTU}	ponct++;
\n		ligne++;
{SPACE}		space++;
%%

int yywrap(void) {
    printf("Nb_mots = %d, Nb_mots_v = %d, Nb_ponct = %d, Nb_ligne = %d, Nb_spaces = %d\n", mot, motV, ponct, ligne, space);
    return 1; }
int main(int argc, char *argv[]) { while (yylex()!=0) ; return 0;}

