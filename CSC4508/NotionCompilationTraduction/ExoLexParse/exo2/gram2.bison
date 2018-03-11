%{
extern int yylex(); 

#include <stdio.h>
int yyerror (char const *message) { 
  fprintf(stderr,"<%s>", message);
  return 0;
}
%}

%error-verbose

%token WORD  '\n'  '.'  ART  TRANS_VERB  COMM_NOUN  PROP_NOUN

%%
Text    	:    /* Mot vide */
	        |       Text    PHRASE
        	        { fprintf(stderr,"[PHRASE]"); }
        	|       Text  '.'
                	{ fprintf(stderr,"."); }
        	|       Text  '\n'
                	{ fprintf(stderr,"\\n\n"); }
;


GRP_NOUN	:	ART COMM_NOUN
		|	ART PROP_NOUN
		|	PROP_NOUN
;

PHRASE		:	GRP_NOUN TRANS_VERB GRP_NOUN '.'
;

%%

int main(void) { return yyparse(); }
