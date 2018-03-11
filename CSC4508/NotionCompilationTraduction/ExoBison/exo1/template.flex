%option nounput noinput

%{
#ifdef FLEXALONE  /* Mode flex seul */
    enum Return_Token_Values { TOK=1000};
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

%%
[0-9]*				sscanf(yytext,"%d", (int *) &yylval); ECHO; return(TOK);
[ \t]				ECHO;
.				{ ECHO; return yytext[0]; }
\n				{ ECHO; return yytext[0]; }
%%

int yywrap (void) {return 1;}
#ifdef FLEXALONE 
  int main(int argc, char *argv[]) { while (yylex()!=0) ; return 0; } 
#endif


