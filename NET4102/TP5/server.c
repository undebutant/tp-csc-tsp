/*
A simple C socket server example with a simple message exchange
*/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>   //write
#include <time.h>
#include <errno.h>

#define MYMSGLEN 2048
#define COSTCTS    23

/*------------------------------------------------------------------------------*/  
int main(int argc, char *argv[])
{

//  int socket_desc, client_sock;
  int socket_desc;
  struct sockaddr_in server, client;
  char client_message[MYMSGLEN];

//Create socket
  socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_desc == -1)
  {
      printf("Could not create socket\n");
      return -1;
  }
  
  printf("Socket master created with descriptor %d\n", socket_desc);

//Prepare the sockaddr_in structure

  server.sin_family=AF_INET;
  server.sin_addr.s_addr=INADDR_ANY;
  server.sin_port = htons(9999);

//Bind

  if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server))<0)
    {
    //print the error message
      perror("Bind failed");
      close(socket_desc);
      return -1;
    }

  puts("Bind done");

// Remet à zéro le buffer

  int ret;
  socklen_t client_size = (socklen_t) sizeof(client);

//Receive messages from client
  while(1) {
    bzero(client_message, MYMSGLEN);
    printf("Attente du message\n");
    ret = recvfrom(socket_desc, client_message, sizeof(client_message) - 1, 0, (struct sockaddr *) &client, &client_size);
    if(ret == -1){
      printf("Erreur\n");
    } else {
      printf("%d bytes received from %s port %hu\n", ret, inet_ntoa(client.sin_addr), ntohs(client.sin_port));
      printf("%s\n", client_message);
    }
    
    ret = sendto(socket_desc, client_message, strlen(client_message), 0, (struct sockaddr *) &client, sizeof(client));
    if(ret == -1){                     
      printf("%s",strerror(errno));
    } else {
      printf("%d bytes sent\n",ret);
    }                                }
}
