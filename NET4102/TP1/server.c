/* Serveur basique se servant de TCP
   le port est passe en argument */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>



int palindrome (char * debut, char * fin, int sizeString)
{
	for (int i = 0 ; i < sizeString - 1 ; i++) {
		if (*debut != *fin) {
			return 0;
		}
		else {
			debut ++;
			fin --;
		}
	}
	return 1;
}


void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	
	struct sockaddr_in serv_addr, cli_addr;
	
	int n;
	
	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockfd < 0)
		error("ERROR opening socket");
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	
	newsockfd = accept(sockfd, 
		(struct sockaddr *) &cli_addr, 
		&clilen);
	
	if (newsockfd < 0) 
		error("ERROR on accept");
	
	bzero(buffer,256);
	n = read(newsockfd,buffer,255);
	
	if (n < 0) error("ERROR reading from socket");
	
	printf("Message recu : %s\n",buffer);

	int strDebut = 0;
	int strFin = 0;
	int pasFini = 1;
	int result = 0;
	int i = 0;

	while ( i <= sizeof(buffer)/sizeof(char) && pasFini) {
		
		if ( buffer[i] == ' ' || buffer[i] == '\0' || buffer[i] == '\n') {
			if ( buffer[i] == '\0' || buffer[i] == '\n') {
				pasFini = 0;
			}
			
			strFin = i - 1;
			result = palindrome (&buffer[strDebut], &buffer[strFin], strFin - strDebut + 1);
			for (int j = strDebut ; j <= strFin; j++) {
				printf ("%c", buffer[j]);
			}
			printf (" %s un palindrome\n", result ? "est" : "n'est pas");
			strDebut = i + 1;
		}

		i++;	
	}

	n = write(newsockfd,"Fin de la reponse",18);

	if (n < 0) error("ERROR writing to socket");
	
	close(newsockfd);
	close(sockfd);
	
	return 0; 
}
