# include <sys/socket.h>
# include <netdb.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

int question ( int sock, char * string )
{
	int code, length, ret ;

	// Send the length of the string.
	length = strlen ( string ) + 1 ;
	code = send ( sock, & length, sizeof ( int ), 0 ) ;
	if ( code == -1 ) {
		perror ( "send" ) ;
		exit ( 1 ) ;
	}

	// Send the string of characters.
	code = send ( sock, string, length, 0 ) ;
	if ( code == -1 ) {
		perror ( "send" ) ;
		exit ( 1 ) ;
	}

	// Wait for the answer.
	code = recv ( sock, & ret, sizeof ( int ), 0 ) ;
	if ( code == -1 ) {
		perror ( "recv" ) ;
		exit ( 1 ) ;
	}

	// Release ressources.
	code = close ( sock ) ;
	if ( code == -1 ) {
		perror ( "close" ) ;
		exit ( 1 ) ;
	}

	return ret ;
}

int connecting ( char * protocol, char * service, char * hostname )
{
	// Socket creation.
	int sock ;
	sock = socket ( AF_INET, SOCK_STREAM, 0 ) ;
	if ( sock == -1 ) {
		perror ( "socket" ) ;
		exit ( 1 ) ;
	}

	// Search for the protocol.
	struct protoent * protoent ;
	protoent = getprotobyname ( protocol ) ;
	if ( ! protoent ) {
		fprintf ( stderr, "Problem with 'getprotobyname'.\n" ) ;
		exit ( 1 ) ;
	}

	// Search for the service.
	struct servent * servent ;
	servent = getservbyname ( service, protoent -> p_name ) ;
	if ( ! servent ) {
		fprintf ( stderr, "Problem with 'getservbyname'.\n" ) ;
		exit ( 1 ) ;
	}

	// Search for the host name.
	struct hostent * hostent ;
	hostent = gethostbyname ( hostname ) ;
	if ( ! hostent ) {
		fprintf ( stderr, "Problem with 'gethostbyname'.\n" ) ;
		exit ( 1 ) ;
	}

	// Initialisation of the sockaddr_in data structure.
	struct sockaddr_in addr ;
	memset ( & addr, 0, sizeof ( struct sockaddr_in ) ) ;
	addr . sin_family = AF_INET ;
	addr . sin_port = servent -> s_port ;
	addr . sin_addr . s_addr = ( ( struct in_addr * ) ( hostent -> h_addr ) ) -> s_addr ;

	// Name the socket.
	int code ;
	code = connect ( sock, ( struct sockaddr * ) & addr, sizeof ( struct sockaddr_in ) ) ;
	if ( code == -1 ) {
		perror ( "connect" ) ;
		exit ( 1 ) ;
	}

	// Return the socket ID.
	return sock ;
}

int main ( int argc, char * argv [ ] )
{
	// Ask for the string of characters.
	char string [ 65536 ];
	printf ( "String to check: " ) ;
	scanf ( "%s", string ) ;

	// Connect to the server.
	int sock ;
	sock = connecting ( "tcp", "iqobject", "localhost" ) ;

	// Ask the question and wait for the answer.
	int ret ;
	ret = question ( sock, string ) ;

	// Display the result.
	printf ( "'%s' is%s a palindrom.\n", string, ( ret ? " " : " not" ) ) ;

	return 0 ;
}
