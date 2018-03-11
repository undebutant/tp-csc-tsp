# include <stdio.h>
# include <string.h>

int palindrome ( char * s )
{
	char * t = s + strlen ( s ) - 1 ;

	while ( * s ++ == * t -- )
		;
	return s >= t ;
}

int main ( int argc, char * argv [ ] )
{
	int i ;

	for ( i = 1 ; i < argc ; i ++ )
		printf (
			"'%s' %s un palindrome.\n",
			argv [ i ],
			palindrome ( argv [ i ] ) ? "est" : "n'est pas"
		) ;

	return 0 ;
}
