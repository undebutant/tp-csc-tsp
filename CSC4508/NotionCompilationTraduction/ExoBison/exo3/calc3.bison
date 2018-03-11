%{
extern int yylex(); 

#include <stdio.h>
#include "proto-color.h"

float tab[26];

int yyerror (char const *message) { 
  fprintf(stderr,RED("<%s>")"\n", message);
  return 0;
}

void print(char * msg) {
    fprintf(stdout,BLUE("%s"),msg);
}
%}

%error-verbose

%token <VALFLOAT> constantefloat
%token <NOMVARIABLE> variable
%type <VALFLOAT> Affectation
%type <VALFLOAT> EXP

%union{
	float VALFLOAT;
	char NOMVARIABLE;
}

/* Pour l'ordre de resolution, on dit que les operandes sont associatifs a gauche via left
   cad que 7 - 1 - 2 + 2 * 3 = ( ( 7 - 1 ) - 2 ) + ( 2 * 3 ) selon priorite et resolution a gauche
   IMPORTANT plus la regle est basse, plus elle est prioritaire
 */
/* On rend - identique a + en terme de priorite */
%left '-' '+'

/* De meme pour * et la division */
%left '*' '/'

%%

Lignes:		/*Vide*/
		| Lignes EXP '\n' { fprintf(stdout,"\nRES=%f\n\n", $2); }
		| Lignes Affectation '\n' { fprintf(stdout, "\n\n"); }
		| Lignes error '\n'{ print("Error syntaxe"); }

EXP:		'(' EXP ')' { $$ = $2; }
		| EXP '+' EXP { $$ = $1 + $3; }
		| EXP '-' EXP { $$ = $1 - $3; }
		| EXP '*' EXP { $$ = $1 * $3; }
		| EXP '/' EXP { $$ = $1 / $3; }
		| '-' EXP { $$ = - $2; }
		| variable { $$ = tab[$1 - 'A']; }
		| constantefloat { $$ = $1; }

Affectation:	variable '=' EXP { tab[$1 - 'A'] = $3; $$ = $3; }

%%
int main(void) { return yyparse(); }
