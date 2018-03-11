# include <stdio.h>


void swap ( int * x, int * y )
{
	int temp ;

	printf ( "swap 1 : x=%d, y=%d\n", *x, *y ) ;

	temp = * x ;
	* x = * y ;
	* y = temp ;

	printf ( "swap 2 : x=%d, y=%d\n", *x, *y ) ;
}

int main ( int argc, char * argv [ ] )
{
	int a, b ;
	
	a = 3 ;
	b = 5 ;
	
	int * pointA = &a;
	int * pointB = &b;

	printf ( "main 1 : a=%d, b=%d\n", a, b ) ;

	swap ( pointA, pointB ) ;

	printf ( "main 2 : a=%d, b=%d\n", a, b ) ;

	return 0 ;
}
