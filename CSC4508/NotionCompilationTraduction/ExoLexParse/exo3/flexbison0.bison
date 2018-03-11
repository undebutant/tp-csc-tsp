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
%token	WORD  '.'

%%
Text:	WORD { print("Word1"); }
	| Text  WORD { print("Word"); }
        | Text '.' { print("Dot"); }
	;
%%
int main(void) { return yyparse(); }
