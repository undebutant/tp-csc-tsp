%option nounput noinput

%{
#include "proto-color.h"  
void echo(char *lex_cat) {
    fprintf(stdout,GREEN("[%s:%s]"), lex_cat, yytext); 
}
void echonl() {
    fprintf(stdout,RED("[\\n]")"\n"); 
}
%}

%s IN_COMMENTS

%%
"/*"			BEGIN(IN_COMMENTS);
<IN_COMMENTS>"*/"	BEGIN(INITIAL);
<IN_COMMENTS>.		;
%%

int yywrap(void) { return 1; }
int main(int argc, char *argv[]) { while (yylex()!=0) ; return 0; } 
