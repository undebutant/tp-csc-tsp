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
	        |       Text    WORD
        	        { fprintf(stderr,"[WORD]"); }
		|	Text    GRP_NOUN
			{ fprintf(stderr,"[GRP_NOUN]"); }
		|	Text    TRANS_VERB
			{ fprintf(stderr,"[TRANS_VERB]"); }
	        |       Text  '.'
        	        { fprintf(stderr,"."); }
	        |       Text  '\n'
       	        	{ fprintf(stderr,"\\n\n"); }
;

GRP_NOUN	:	ART COMM_NOUN
		|	ART PROP_NOUN
		|	PROP_NOUN
;


%%

int main(void) { return yyparse(); }
