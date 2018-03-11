%%
[a-zA-Z][a-zA-Z0-9]*   printf("[ %s ]", yytext );
%%
int yywrap(void) { return 1; }
int main(int argc, char *argv[]) { while (yylex()!=0) ; return 0; } 
