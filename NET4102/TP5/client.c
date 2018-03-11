/*
A simple C socket client example with a simple message exchange
*/

#include <stdio.h> //printf
#include <string.h> //strlen
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include <errno.h>

#define MYMSGLEN 2048

/*------------------------------------------------------------------------------*/  
int main (int argc, char *argv[]){
  
  int sock;
  struct sockaddr_in server;
  char message[MYMSGLEN], response[MYMSGLEN];
  
  //Create socket
  sock=socket(AF_INET, SOCK_DGRAM, 0);

  if (sock == -1)
  {
    printf("Could not create socket\n");
    return -1;
  }

  puts("Socket created\n");

  int rset;
  int broadcastEnable = 1;
  rset = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
  if(rset == -1){
    printf("%s",strerror(errno));
  } else {
    printf("Setsock success\n");
  }


  //server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family=AF_INET;
  server.sin_port=htons(9999);

  int ret;
  socklen_t server_size = (socklen_t) sizeof(server);
  while(1){
    bzero(response, MYMSGLEN);
    printf("Entrer message:\n");
    scanf("%s",message);
    printf("Envoi\n");
    ret = sendto(sock, message, strlen(message), 0, (struct sockaddr *) &server, sizeof(server));
    if(ret == -1){
      printf("%s",strerror(errno));
    } else {
      printf("%d bytes sent\n",ret);
    }
   
    ret = recvfrom(sock, response, sizeof(response) - 1, 0, (struct sockaddr *) &server, &server_size);
    if(ret == -1){
      printf("%s",strerror(errno));
    } else {
      printf("%d bytes received from %s port %hu\n", ret, inet_ntoa(server.sin_addr), ntohs(server.sin_port));
      printf("%s\n", response);
    }
  }
}

