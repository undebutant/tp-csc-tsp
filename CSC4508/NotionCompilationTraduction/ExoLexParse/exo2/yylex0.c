#include <ctype.h>
#include <stdio.h>
#include <string.h>

#ifdef FLEXALONE
	#define WORD 1001
#else
	#include "gram.tab.h"
#endif

// Defini automatiquement par Bison, ne pas mettre cette ligne

// #define WORD 1001

int yylex(){
  char  yytext[256];
  int   yylen = 0;
  int   c;

  while( (c=getchar()) == ' ' );

  if( c==EOF  ) { return c; }

  if( c=='\n' ) { fprintf(stderr,"[\\n]%c", c); return c;}
  if( c=='.' )  { fprintf(stderr,"[%c]", c); return c;}

  while( isalpha(c) ) {
    yytext[yylen++] = c;
    c = getchar();
  }
  if( c != EOF ) ungetc(c,stdin);
  yytext[yylen] = '\0';

  fprintf(stderr,"[WORD: %s]", yytext);
  return WORD;
}

// On ne peut avoir qu'un unique main, celui de bison
// (l'autre sert uniquement si on veut faire l'analyseur)

// int main(int argc, char *argv[]) { while (yylex()!=EOF) ; return 0; }
