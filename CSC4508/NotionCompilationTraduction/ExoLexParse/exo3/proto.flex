%option nounput noinput

%{
#ifdef FLEXALONE /* Mode flex seul */
  enum Return_Token_Values { ANY=1000, UNK};
#else            /* couplage flex/bison */
  #include "yyparse.h"  
#endif

#include "proto-color.h"
void echo(char *lex_cat) {
  fprintf(stdout,GREEN("[%s:%s]"), lex_cat, yytext); 
}
void echonl() {
  fprintf(stdout,BLUE("[\\n]")"\n"); 
}
%}

%%
[a-z]*		echo("ANY"); return(ANY);
\n		echonl();    return(yytext[0]);
.		echo("UNK"); return(UNK);
%%

int yywrap (void) {return 1;}
#ifdef FLEXALONE 
  int main(int argc, char *argv[]) { while (yylex()!=0) ; return 0; } 
#endif
