# include <sys/socket.h>
# include <netdb.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>



void answer ( int sock, struct sockaddr_in addr, socklen_t len )
{
	// Receive the length of the string.
	int code, length ;
	code = recv ( sock, & length, sizeof ( int ), 0 ) ;
	if ( code == -1 ) {
		perror ( "recv" ) ;
		exit ( 1 ) ;
	}

	// Allocate memory for the string.
	char * string ;
	string = ( char * ) malloc ( ( length + 1 ) * sizeof ( char ) ) ;
	if ( ! string ) {
		fprintf ( stderr, "Cannot allocate memory for string.\n" ) ;
		exit ( 1 ) ;
	}

	// Receive the string of characters.
	code = recv ( sock, string, length, 0 ) ;
	if ( code == -1 ) {
		perror ( "recv" ) ;
		exit ( 1 ) ;
	}

	// ToDo

	// Return the answer.
	code = send ( sock, & ret, sizeof ( int ), 0 ) ;
	if ( code == -1 ) {
		perror ( "send" ) ;
		exit ( 1 ) ;
	}

	// Release ressources.
	free ( string ) ;
	code = close ( sock ) ;
	if ( code == -1 ) {
		perror ( "close" ) ;
		exit ( 1 ) ;
	}
}

int naming ( char * protocol, char * service, char * hostname )
{
	// Socket creation.
	int sock ;
	sock = socket ( AF_INET, SOCK_STREAM, 0 ) ;
	if ( sock == -1 ) {
		perror ( "socket" ) ;
		exit ( 1 ) ;
	}

	// Reuse the same port (useful when developing...).
	int code, enable = 1;
	code = setsockopt ( sock, SOL_SOCKET, SO_REUSEADDR, & enable, sizeof ( int ) ) ;
	if (code == -1 ) {
		perror ( "setsockopt" ) ;
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
	code = bind ( sock, ( struct sockaddr * ) & addr, sizeof ( struct sockaddr_in ) ) ;
	if ( code == -1 ) {
		perror ( "bind" ) ;
		exit ( 1 ) ;
	}

	// Return the socket ID.
	return sock ;
}

int main ( int argc, char * argv [ ] )
{
	// Name the socket.
	int sock ;
	sock = naming ( "tcp", "iqobject", "localhost" ) ;

	// Set up listening on the port.
	int code ;
	code = listen ( sock, 5 ) ;
	if ( code == -1 ) {
		perror ( "listen" ) ;
		exit ( 1 ) ;
	}

	// Wait for incoming connections and deal with the clients.
	int new ;
	socklen_t len ;
	struct sockaddr_in addr ;
	for ( ; ; ) {
		len = sizeof ( struct sockaddr_in ) ;
		new = accept ( sock, ( struct sockaddr * ) & addr, & len ) ;
		if ( new == -1 ) {
			perror ( "accept" ) ;
			exit ( 1 ) ;
		}
		answer ( new, addr, len ) ;
	}

	// This point in the program will never be reached.
	return 0 ;
}
