%{
extern int yylex(); 

#include <stdio.h>
#include "proto-color.h"
int yyerror (char const *message) { 
  fprintf(stderr,RED("<%s>")"\n", message);
  return 0;
}

void print(char * msg) {
    fprintf(stdout,BLUE("%s"),msg);
}
%}

%error-verbose

%token <VALINT> constante
%type <VALINT> EXP

%union{
	int VALINT;
}

%%

Lignes:		/*Vide*/
		| Lignes EXP '\n' { fprintf(stdout,"\nRES=%d\n\n", $2); }
		| Lignes error '\n'{ print("Error syntaxe"); }

EXP:		'(' EXP ')' { $$ = $2; }
		| '(' EXP '+' EXP ')' { $$ = $2 + $4; }
		| '(' EXP '-' EXP ')' { $$ = $2 - $4; }
		| '(' EXP '*' EXP ')' { $$ = $2 * $4; }
		| '(' EXP '/' EXP ')' { $$ = $2 / $4; }
		| '(' EXP '%' EXP ')' { $$ = $2 % $4; }
		| constante { $$ = $1; }

%%
int main(void) { return yyparse(); }
