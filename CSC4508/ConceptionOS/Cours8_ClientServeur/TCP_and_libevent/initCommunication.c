/***********************/
/* initCommunication.c */
/***********************/

#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h> // To get TCP_NODELAY declaration
#include <netdb.h>
#include <unistd.h>

#include "initCommunication.h"
#include "client-serveur.h"

// Fonction utilisee cote client
// Elle se connecte au point d'acces TCP correspondant au couple
// hote HOST / port PORT.
// Renvoie le file descriptor de ce point d'acces (ou bien quitte
// le programme s'il y a eu une erreur).
int connexionPointDAcces() {
  //
  // The following code is an adaptation from the example in man getaddrinfo
  //

  int fdConnexion;
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int s;
  int rc;
  int status=1;

  // Obtain address(es) matching host/port
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM; /* Stream socket */
  hints.ai_flags = 0;
  hints.ai_protocol = 0;          /* Any protocol */
  
  s = getaddrinfo(HOST, PORT, &hints, &result);
  if (s != 0) {
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, 
		  "getaddrinfo on hostname \"%s\": %s\n", HOST, gai_strerror(s));
  }
  
  // getaddrinfo() returns a list of address structures.
  // Try each address until we successfully connect(2).
  // If socket(2) (or connect(2)) fails, we (close the socket
  // and) try the next address. */
  
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    fdConnexion = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (fdConnexion == -1)
      continue;

    rc = connect(fdConnexion , rp->ai_addr, rp->ai_addrlen);
    if (rc >= 0)
      break;                  /* Success */

    if (close(fdConnexion) < 0)
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "close");
  }

  freeaddrinfo(result);           /* No longer needed */

  if (rp == NULL) {               /* No address succeeded */
    fprintf(stderr, "Could not connect to \"%s\" on port \"%s\" ==> Abort\n", HOST, PORT);
    exit(EXIT_FAILURE);
  }

  // We set TCP_NODELAY flag so that packets sent on this TCP connection
  // will not be delayed by the system layer
  if (setsockopt(fdConnexion, IPPROTO_TCP, TCP_NODELAY, &status,sizeof(status)) < 0){
    //free(aComm);
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "setsockopt");
  }

  return fdConnexion;
}

// Fonction utilisee cote serveur
// Elle cree le point d'acces TCP correspondant au port PORT.
// Renvoie le file descriptor de ce point d'acces (ou bien quitte
// le programme s'il y a eu une erreur).
int creationPointDAcces() {
  //
  // (The following code is an adaptation from the example in man getaddrinfo)
  //

  int fdPointDAcces, s, on = 1;
  struct addrinfo hints;
  struct addrinfo *result, *rp;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM;/* Stream socket */
  hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
  hints.ai_protocol = 0;          /* Any protocol */
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  s = getaddrinfo(NULL, PORT, &hints, &result);
  if (s != 0) {
    fprintf(stderr, "%s:%d: getaddrinfo: %s\n", __FILE__, __LINE__, gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  /* getaddrinfo() returns a list of address structures.
     Try each address until we successfully bind(2).
     If socket(2) (or bind(2)) fails, we (close the socket
     and) try the next address. */

  for (rp = result; rp != NULL; rp = rp->ai_next) {
    fdPointDAcces = socket(rp->ai_family, rp->ai_socktype,
		 rp->ai_protocol);
    if (fdPointDAcces == -1)
      continue;

    // We position the option to be able to reuse a port in case this port
    // was already used in a near past by another process
    if (setsockopt(fdPointDAcces, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
      continue;

    if (bind(fdPointDAcces, rp->ai_addr, rp->ai_addrlen) == 0)
      break;                  /* Success */

    if (close(fdPointDAcces) < 0)
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "close");      
  }

  if (rp == NULL)               /* No address succeeded */
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "could not bind");

  freeaddrinfo(result);           /* No longer needed */

  // Nous acceptons d'avoir au maximum 5 connexions en attente sur
  // le point d'acces
  if( listen (fdPointDAcces,5) < 0 )
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "listen");

  return fdPointDAcces;
}

