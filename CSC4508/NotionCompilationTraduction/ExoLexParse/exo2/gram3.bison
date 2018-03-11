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
LISTE_PHRASE	:	// liste vide
		|	LISTE_PHRASE PHRASE
		|	LISTE_PHRASE '\n'
			{ fprintf(stderr,"[RETOUR_LIGNE_LISTE_PHRASE]\n"); }
;


GRP_NOUN	:	ART COMM_NOUN
		|	ART PROP_NOUN
		|	PROP_NOUN
;

PHRASE		:	GRP_NOUN TRANS_VERB GRP_NOUN '.'
;


%%

int main(void) { return yyparse(); }
