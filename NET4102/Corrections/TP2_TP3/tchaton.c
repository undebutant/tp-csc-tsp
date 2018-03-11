# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <netdb.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <unistd.h>

char * strtime ( )
{
	static char time [ 9 ] ;

	// Get the number of seconds since Jan. 1st, 1970.
	int code ;
	struct timeval tv ;
	code = gettimeofday ( & tv, NULL ) ;
	if ( code == -1 ) {
		perror ( "gettimeofday" ) ;
		pthread_exit ( ( void * ) 1 ) ;
	}

	// Translate the number of seconds into date.
	struct tm * tm ;
	tm = localtime ( & tv . tv_sec ) ;
	sprintf ( time, "%02d:%02d:%02d", tm -> tm_hour, tm -> tm_min, tm -> tm_sec ) ;

	// Return a pointer to the static string os characters.
	return time ;
}

int socket_setup ( char * host, int port, struct sockaddr_in * addr )
{
	// Socket creation.
	int sock ;
	sock = socket ( AF_INET, SOCK_STREAM, 0 ) ;
	if ( sock == -1 ) {
		perror ( "socket" ) ;
		exit ( 1 ) ;
	}

	// Search for the host name.
	struct hostent * hostent ;
	hostent = gethostbyname ( host ) ;
	if ( ! hostent ) {
		fprintf ( stderr, "Problem with 'gethostbyname'.\n" ) ;
		exit ( 1 ) ;
	}

	// Initialisation of the sockaddr_in data structure.
	memset ( addr, 0, sizeof ( struct sockaddr_in ) ) ;
	addr -> sin_family = AF_INET ;
	if ( port )
		addr -> sin_port = htons ( port ) ;
	addr -> sin_addr . s_addr = ( ( struct in_addr * ) ( hostent -> h_addr ) ) -> s_addr ;

	// Return the socket ID.
	return sock ;
}

# define END_MESSAGE "quit\n"

# define MAX_BUF	65536
# define MAX_PSEUDO	256

# define MAX_READER		16
# define MAX_WRITER		16

int sock_reader [ MAX_READER ] ;
char * sock_writer [ MAX_WRITER ] ;

# define MESSAGE_HELLO		1
# define MESSAGE_NORMAL		2
# define MESSAGE_GOODBYE	3

void broadcast_message ( char * pseudo, int type, char * message )
{
	// Create the message to send to all readers.
	char buf [ MAX_BUF ] ;
	switch ( type ) {
	case MESSAGE_HELLO :
		sprintf ( buf, "[%s] '%s' joined the chat.\n", strtime ( ), pseudo ) ;
		break ;
	case MESSAGE_NORMAL :
		sprintf ( buf, "[%s] '%s' says: %s", strtime ( ), pseudo, message ) ;
		break ;
	case MESSAGE_GOODBYE :
		sprintf ( buf, "[%s] '%s' left the chat.\n", strtime ( ), pseudo ) ;
		break ;
	}
	printf ( buf ) ;

	// Broadcast the message to all readers.
	int i ;
	for ( i = 0 ; i < MAX_READER ; i ++ )
		if ( sock_reader [ i ] ) {
			int code ;
			code = send ( sock_reader [ i ], buf, strlen ( buf ) + 1, 0 ) ;
			if ( code == -1 ) {
				perror ( "send" ) ;
				pthread_exit ( ( void * ) 1 ) ;
			}
		}
}

void serve_writer ( int sock )
{
	// Read the pseudo from the writer.
	char pseudo [ MAX_PSEUDO ] ;
	int code ;
	code = recv ( sock, pseudo, MAX_PSEUDO, 0 ) ;
	if ( code == -1 ) {
		perror ( "recv" ) ;
		pthread_exit ( ( void * ) 1 ) ;
	}

	// Look for another thread having the same pseudo.
	int ack, i ;
	for ( ack = 1, i = 0 ; i < MAX_WRITER ; i ++ )
		if ( sock_writer [ i ] && ! strcmp ( pseudo, sock_writer [ i ] ) )
			ack = 0 ;

	// Send the acknowledgement.
	code = send ( sock, & ack, sizeof ( int ), 0 ) ;
	if ( code == -1 ) {
		perror ( "send" ) ;
		pthread_exit ( ( void * ) 1 ) ;
	}

	// Stop the writer if pseudo already in use.
	if ( ! ack )
		pthread_exit ( ( void * ) 1 ) ;

	// Look for an available slot in the sock_writer list.
	for ( i = 0 ; i < MAX_WRITER && sock_writer [ i ] ; i ++ )
		;
	if ( i == MAX_WRITER ) {
		fprintf ( stderr, "sock_writer list overflow.\n" ) ;
		pthread_exit ( ( void * ) 1 ) ;
	}
	sock_writer [ i ] = pseudo ;

	// New writer said hello !
	broadcast_message ( pseudo, MESSAGE_HELLO, NULL ) ;

	char message [ MAX_BUF ] ;
	do {
		// Read the next message from the writer.
		code = recv ( sock, message, MAX_BUF, 0 ) ;
		if ( code == -1 ) {
			perror ( "recv" ) ;
			pthread_exit ( ( void * ) 1 ) ;
		}

		// Forward the message to all readers.
		if ( strcmp ( message, END_MESSAGE ) )
			broadcast_message ( pseudo, MESSAGE_NORMAL, message ) ;
	} while ( strcmp ( message, END_MESSAGE ) ) ;

	// Writer says good bye :-(
	broadcast_message( pseudo, MESSAGE_GOODBYE, NULL ) ;

	// Reset pseudo.
	sock_writer [ i ] = NULL ;

	// Close the socket.
	close ( sock ) ;
}

void serve_reader ( int sock )
{
	// Look for an available slot in the sock_reader list.
	int i ;
	for ( i = 0 ; i < MAX_READER && sock_reader [ i ] ; i ++ )
		;
	if ( i == MAX_READER ) {
		fprintf ( stderr, "sock_reader list overflow.\n" ) ;
		pthread_exit ( ( void * ) 1 ) ;
	}
	sock_reader [ i ] = sock ;

	// Ideally, one should wait for the reader to disconnect...
}

# define READER		0
# define WRITER		1

void * service ( void * arg )
{
	// Get socket ID.
	int sock = ( long ) arg ;

	// No need to join at the end.
	pthread_detach ( pthread_self ( ) ) ;

	// Read the client role.
	int code, role ;
	code = recv ( sock, & role, sizeof ( int ), 0 ) ;
	if ( code == -1 ) {
		perror ( "recv" ) ;
		pthread_exit ( ( void * ) 1 ) ;
	}
	if ( role != READER && role != WRITER ) {
		close ( sock ) ;
		return ( void * ) 1 ;
	}

	// Start the good service for the client.
	if ( role )
		serve_writer ( sock ) ;
	else
		serve_reader ( sock ) ;

	return ( void * ) 0 ;
}

void server ( )
{
	// Socket set up.
	int sock ;
	struct sockaddr_in addr ;
	sock = socket_setup ( "localhost", 0, & addr ) ;

	// Name the socket.
	int code ;
	socklen_t len ;
	len = sizeof ( struct sockaddr_in ) ;
	code = bind ( sock, ( struct sockaddr * ) & addr, len ) ;
	if ( code == -1 ) {
		perror ( "bind" ) ;
		exit ( 1 ) ;
	}

	// Local port number.
	struct sockaddr_in local ;
	memset ( & local, 0, sizeof ( struct sockaddr_in ) ) ;
	code = getsockname ( sock, ( struct sockaddr * ) & local, & len ) ;
	if ( code == -1 ) {
		perror ( "getsockname" ) ;
		exit ( 1 ) ;
	}

	// Display information about the server.
	printf ( "Hostname: %s\n", inet_ntoa ( local . sin_addr ) ) ;
	printf ( "Port number: %d\n", ntohs ( local . sin_port ) ) ;

	// Set up listening on the port.
	code = listen ( sock, 5 ) ;
	if ( code == -1 ) {
		perror ( "listen" ) ;
		exit ( 1 ) ;
	}

	int new_sock ;
	socklen_t new_len ;
	struct sockaddr_in new_addr ;
	for ( ; ; ) {
		// Wait for incoming connections.
		new_len = sizeof ( struct sockaddr_in ) ;
		new_sock = accept ( sock, ( struct sockaddr * ) & new_addr, & new_len ) ;
		if ( new_sock == -1 ) {
			perror ( "accept" ) ;
			exit ( 1 ) ;
		}

		// Start the thread to deal with the client.
		pthread_t tid ;
		pthread_create ( & tid, NULL, service, ( void * ) ( long ) new_sock ) ;
	}
}

// If pseudo is null, the client is a reader, otherwise it is a writer.
int connecting ( char * host, char * port, char * pseudo )
{
	// Socket set up.
	int sock ;
	struct sockaddr_in addr ;
	sock = socket_setup ( host, atoi ( port ), & addr ) ;

	// Connection to the server.
	int code ;
	code = connect ( sock, ( struct sockaddr * ) & addr, sizeof ( struct sockaddr_in ) ) ;
	if ( code == -1 ) {
		perror ( "connect" ) ;
		exit ( 1 ) ;
	}

	// Tell the server I am a reader.
	int role = ( pseudo ? WRITER : READER ) ;
	code = send ( sock, & role, sizeof ( int ), 0 ) ;
	if ( code == -1 ) {
		perror ( "send" ) ;
		exit ( 1 ) ;
	}

	// Send pseudo to the server and wait for acknowledgement.
	if ( pseudo ) {
		int ack ;
		code = send ( sock, pseudo, strlen ( pseudo ) + 1, 0 ) ;
		if ( code == -1 ) {
			perror ( "send" ) ;
			exit ( 1 ) ;
		}
		code = recv ( sock, & ack, sizeof ( int ), 0 ) ;
		if ( code == -1 ) {
			perror ( "recv" ) ;
			exit ( 1 ) ;
		}
		if ( ! ack ) {
			fprintf ( stderr, "Pseudo already in use.\n" ) ;
			exit ( 1 ) ;
		}
	}

	return sock ;
}

void reader ( char * host, char * port )
{
	// Connect to the server.
	int sock = connecting ( host, port, NULL ) ;

	char buf [ MAX_BUF ] ;
	for ( ; ; ) {
		// Read messages from the server...
		int code ;
		code = recv ( sock, buf, sizeof ( buf ), 0 ) ;
		if ( code == -1 ) {
			perror ( "recv" ) ;
			exit ( 1 ) ;
		}

		// ... and display them on the terminal.
		printf ( "%s", buf ) ;
	}

	// One should never reach this point.

	// Close the socket.
	close ( sock ) ;
}

void writer ( char * host, char * port, char * pseudo )
{
	// Connect to the server.
	int sock = connecting ( host, port, pseudo ) ;

	char buf [ MAX_BUF ] ;
	do {
		// Get the message.
		printf ( "> " ) ;
		fgets ( buf, MAX_BUF, stdin ) ;

		// Send the message to the server.
		int code ;
		code = send ( sock, buf, strlen ( buf ) + 1, 0 ) ;
		if ( code == -1 ) {
			perror ( "send" ) ;
			exit ( 1 ) ;
		}
	} while ( strcmp ( buf, END_MESSAGE ) ) ;

	// Close the socket.
	close ( sock ) ;
}

int main ( int argc, char * argv [ ] )
{
	int usage = 1 ;
	if ( argc > 1 )
		if ( argv [ 1 ] [ 0 ] == '-' && argv [ 1 ] [ 2 ] == '\0' )
			switch ( argv [ 1 ] [ 1 ] ) {
			case 's' :
				if ( argc == 2 )
					server ( ) ;
				usage = 0 ;
				break ;
			case 'w' :
				if ( argc == 5 )
					writer ( argv [ 2 ], argv [ 3 ], argv [ 4 ] ) ;
				usage = 0 ;
				break ;
			case 'r' :
				if ( argc == 4 )
					reader ( argv [ 2 ], argv [ 3 ] ) ;
				usage = 0 ;
				break ;
			}

	if ( usage ) {
		fprintf ( stderr, "Usage: %s -s\n", argv [ 0 ] ) ;
		fprintf ( stderr, "       %s -w <host> <#port> <pseudo>\n", argv [ 0 ] ) ;
		fprintf ( stderr, "       %s -r <host> <#port>\n", argv [ 0 ] ) ;
		return 1 ;
	}

	return 0 ;
}
