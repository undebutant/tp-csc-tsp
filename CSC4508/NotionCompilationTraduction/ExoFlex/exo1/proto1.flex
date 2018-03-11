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

%%
\n					{ echonl(); }
(for|while|if)				{ echo("KeyWord"); }
(\+|\*|\/|\-|\=)+			{ echo("Operator"); }
(\<|\>)+				{ echo("Compare"); }
(\(|\)|\{|\}|;|\,|\[|\]|\.)+		{ echo("Separator"); }
[0-9]*\.[0-9]*				{ echo("Float"); }
[0-9]+					{ echo("Integer"); }
[a-zA-Z]+				{ echo("Word"); }
[a-zA-Z][a-zA-Z0-9]+			{ echo("Variable"); }
[ ]+					{ echo("Spacing"); }
.					{ echo("Unknown"); }
%%

int yywrap(void) { return 1; }
int main(int argc, char *argv[]) { while (yylex()!=0) ; return 0; } 
