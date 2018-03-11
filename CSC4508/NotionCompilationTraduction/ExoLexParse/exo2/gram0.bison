%{
extern int yylex(); 

#include <stdio.h>
int yyerror (char const *message) { 
  fprintf(stderr,"<%s>", message);
  return 0;
}
%}

%error-verbose

%token WORD  '\n'  '.'

%%
Text    :    /* Mot vide */
        |       Text    WORD
                { fprintf(stderr,"WORD "); }
        |       Text  '.'
                { fprintf(stderr,"."); }
        |       Text  '\n'
                { fprintf(stderr,"\\n\n"); }
        ;
%%

int main(void) { return yyparse(); }
