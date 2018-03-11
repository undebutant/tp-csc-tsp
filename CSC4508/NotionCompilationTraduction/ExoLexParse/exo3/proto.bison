%{
extern int yylex(); 

#include <stdio.h>
#include "proto-color.h"
int yyerror (char const *message) { 
  fprintf(stderr,RED("<%s>")"\n", message);
  return 0;
}
%}

%error-verbose
%token ANY UNK '\n'

%% 
Line	:
	| Line Expr '\n' 
	| Line error '\n' { yyerrok; }
	;
Expr    : 
	| ANY
	;
%%
int main(void) { return yyparse(); }
