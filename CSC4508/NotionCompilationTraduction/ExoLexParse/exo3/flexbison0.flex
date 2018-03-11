%option nounput noinput

%{
#ifdef FLEXALONE  /* Mode flex seul */
    enum Return_Token_Values { WORD=1000};
#else             /* couplage flex/bison */
  #include "yyparse.h"  
#endif

#include "proto-color.h"
void echo(char *lex_cat) {
  fprintf(stdout,GREEN("[%s:%s]"), lex_cat, yytext); 
}
void echonl() {
  fprintf(stdout,GREEN("[\\n]")"\n"); 
}
%}

LETTRE [a-zA-Z]

%%
{LETTRE}{LETTRE}*	|
\"[^ \n]*\"		{ echo("WORD"); return WORD; }
\.                      { echo("DOT"); return yytext[0];}
[ \t]			{ ECHO; }
\n                      {echonl();}
.			{ ECHO;}
%%

int yywrap (void) {return 1;}
#ifdef FLEXALONE 
  int main(int argc, char *argv[]) { while (yylex()!=0) ; return 0; } 
#endif


