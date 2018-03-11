%{
extern int yylex(); 

#include <stdio.h>
#include <math.h>
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


%token <VALINT> TOK

%type <VALINT> IntPut


%union{
        int VALINT;
}

%%

ListCarac:	/* Mot vide */
		| ListCarac 'a' Lista '\n' { print("Ligne a OK\n"); }
		| ListCarac 'b' Listb '\n' { print("Ligne b OK\n"); }
		| ListCarac error '\n' { yyerror("Ligne fausse"); }
		
Lista:		IntPut

Listb:		IntPut IntPut

IntPut:		TOK


%%
int main(void) { return yyparse(); }
