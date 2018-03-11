/*
A simple C socket client example with a simple message exchange
*/

#include <stdio.h> //printf
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>

#define MYMSGLEN 2048

/*------------------------------------------------------------------------------*/  
int main (int argc, char *argv[]){
  
  int sock;
  int rec_res;
  struct sockaddr_in server;
  char message[MYMSGLEN], server_reply[MYMSGLEN+8];

  struct messageCost{
    uint32_t cost;
    uint32_t msgLen;
    char buff[MYMSGLEN]; 
  } *msg;
  
  
  
  //Create socket
  sock=socket(AF_INET, SOCK_STREAM, 0);

  if (sock == -1)
  {
    printf("Could not create socket\n");
    return -1;
  }

  puts("Socket created\n");


  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family=AF_INET;
  server.sin_port=htons(8888);


  //connect to remote server
  int res;

  if((res=connect(sock,(struct sockaddr *) &server, sizeof(server)))<0)
  {
    perror("Connect failed error\n");
    close(sock);
    return -1;
  }

 
  puts("Connection established with client...\n");

//Keep communicating with server

  while(1)
  {
    printf("\n----------------------------------------------------------------------\n");
    printf("Please type a message to transfer for processing : ");
    scanf("%s",message);
    printf("----------------------------------------------------------------------\n");
    //send the message
    if (send(sock, message,strlen(message),0)<0)
      {
	puts("Send failed\n");
	close(sock);
	return -1;
      }



//Receive a reply from the server
memset(server_reply,0,MYMSGLEN);
  if((rec_res=recv(sock,server_reply,MYMSGLEN+8,0))<0)
    {
      puts("recv failed\n");
      close(sock);
      return -1;
    }
    printf("\nSize of server message received : %d\n",rec_res);
  if(rec_res>0)
  {
    puts("Server reply: ");
    msg=(struct messageCost *) (server_reply);
    msg->cost=ntohl(msg->cost);
    msg->msgLen=ntohl(msg->msgLen);
    printf("----------------------------------------------------------------------\n");
    printf("Message Length: %d, with a cost of : %.2f Euros\n",msg->msgLen, ((float)msg->cost/100));
    puts(msg->buff);
    printf("----------------------------------------------------------------------\n");
  }	
  else
    { //0 char received
      puts("Connection closed by server \n ");
      close(sock);
      return 0;
    }	
  }

}	





