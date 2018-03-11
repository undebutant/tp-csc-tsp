#include <ctype.h>
#include <stdio.h>
#include <string.h>

#ifdef FLEXALONE
  #define WORD 1001
  #define ART  1002
  #define TRANS_VERB 1003
  #define COMM_NOUN 1004
  #define PROP_NOUN 1005
#else
  #include "gram.tab.h"
#endif

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

  if( !strcmp(yytext,"a")  ||
      !strcmp(yytext,"an") ||
      !strcmp(yytext,"the") )
    {
      fprintf(stderr, "[ART: %s]", yytext);
      return(ART);
    }
  if( !strcmp(yytext,"cut") ||
      !strcmp(yytext,"cuts") )
    {
      fprintf(stderr, "[TRANS_VERB: %s]", yytext);
      return(TRANS_VERB);
    }
  if( !strcmp(yytext,"bill") ||
      !strcmp(yytext,"bills") )
    {
      fprintf(stderr, "[COMM_NOUN: %s]", yytext);
      return(COMM_NOUN);
    }
  if( !strcmp(yytext,"Bell") ||
      !strcmp(yytext,"Verizon") )
    {
      fprintf(stderr, "[PROP_NOUN: %s]", yytext);
      return(PROP_NOUN);
    }
  
  fprintf(stderr,"[UNK WORD: %s]", yytext);
  return WORD;
}

#ifdef FLEXALONE
int main(int argc, char *argv[]) { while (yylex()!=EOF) ; return 0; } 
#endif
