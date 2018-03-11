/*
A simple C socket server example with a simple message exchange
*/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>   //write
#include <time.h>

#define MYMSGLEN 2048
#define COSTCTS    23

/*------------------------------------------------------------------------------*/  
int main(int argc, char *argv[])
{

  int socket_desc, client_sock;
  int socket_size,read_size;
  struct sockaddr_in server, client;
  char client_message[MYMSGLEN];
//variables for setsockopt
  int code, enable=1;
  
  struct messageCost{
    uint32_t cost;
    uint32_t msgLen;
    char buff[MYMSGLEN]; 
  } msg;

//variables for select
  fd_set setOfSocketDescr;
  FD_ZERO(&setOfSocketDescr);

  struct timeval timerLimit;
  timerLimit.tv_sec = 10; 
  timerLimit.tv_usec = 0;

  int clientList[5];

  for (int i = 0 ; i < 5 ; i++) {
    clientList[i] = -1;
  }

  int selectActivity;
  int maxSockDescr;

//Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1)
  {
      printf("Could not create socket\n");
      return -1;
  }
  
  clientList[0] = socket_desc;

  printf("Socket master created with descriptor %d\n", socket_desc);

//Prepare the sockaddr_in structure

  server.sin_family=AF_INET;
  server.sin_addr.s_addr=INADDR_ANY;
  server.sin_port = htons(8888);

  //Reuse the same port 

  code=setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR,&enable, sizeof(int));
  if (code == -1)
  {
    perror("setsockopt");
    return -1;
  }

  printf("Server-setsockopt() is OK...\n");

//Bind

  if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server))<0)
    {
    //print the error message
      perror("Bind failed");
      close(socket_desc);
      return -1;
    }

  puts("Bind done");

//Listen
  listen(socket_desc,3);


  bzero(client_message, MYMSGLEN);

//Receive messages from client
  while(1) {
    
    FD_ZERO(&setOfSocketDescr);

    timerLimit.tv_sec = 30; 
    timerLimit.tv_usec = 0;

    maxSockDescr = 0;

    for (int j = 0 ; j < 5 ; j++) {
      if (clientList[j] >= 0) {
        FD_SET(clientList[j], &setOfSocketDescr);
	printf("Ajout du socket %d\n", clientList[j]);

	if(clientList[j] > maxSockDescr) {
	  maxSockDescr = clientList[j];
	}
      }
    }


    selectActivity = select(maxSockDescr + 1, &setOfSocketDescr, NULL, NULL, &timerLimit);
    
    if(selectActivity == -1) {
      perror("select error");
    }
    else if (selectActivity > 0) {
      // Activité detectee
      printf("\nActivité détectée\n");
      // Accept new connection on master_socket
      if (FD_ISSET(clientList[0], &setOfSocketDescr)) {
      
	printf("Nouvelle connection sur le socket master\n");
        socket_size=sizeof(struct sockaddr_in);

        client_sock = accept(socket_desc, (struct sockaddr *) &client,(socklen_t*) &socket_size);
        if(client_sock<0) {
          close(socket_desc);
          perror("accept failed");
          return -1;
        }
        puts ("New connection accepted\n");
        
	for(int l = 1 ; l < 5 ; l++) {
	  if(clientList[l] < 0) {
	    clientList[l] = client_sock;
	    break;
	  }
	}
      }


      // Deal with other clients data if exist	    
      for (int k = 1 ; k < 5 ; k++) {
	if(clientList[k] >= 0) {
          if(FD_ISSET(clientList[k], &setOfSocketDescr)) {

	    printf("Nouvelles données venant du socket %d\n", clientList[k]);
	  
	    bzero(client_message, MYMSGLEN);
	
	    client_sock = clientList[k];
	    read_size = recv(client_sock,client_message,MYMSGLEN,0);
	  
	    if(read_size > 0) {
	      printf("\n----------------------------------------------------------------------\n");
              printf("Msg received from socket %d : %s, size of the message received :  %d \n", client_sock, client_message, read_size);
              printf("----------------------------------------------------------------------\n\n");
    
  	      // Échappement si quit# reçu
	      if(!memcmp("quit#",client_message,5)) {
	        printf("\nShuting down the connection with socket_descr %d\n\n", client_sock);
	        close(client_sock);
	        for(int i = 1 ; i < 5 ; i++) {
		  if(clientList[i] == client_sock) {
		    clientList[i] = -1;
		    break;
		  }
	        }
	      }
    
  	      // Création de la struct cout du message
              msg.cost=htonl((uint32_t) (COSTCTS * read_size));
              msg.msgLen=htonl((uint32_t)read_size);
              memcpy(msg.buff,client_message,read_size);

	      // Envoi de la struct
              send(client_sock,(char*)&msg, read_size+sizeof(uint32_t)+sizeof(uint32_t),0);
            }
	    else if(read_size==0) {
              printf("client %d disconnected", client_sock);
              //fflush(stdout);
            }
            else if(read_size== -1) {
              perror("recv failed");
            }
          }
        }
      }
    }
    else {
      printf("\nPas d'activité les 10 dernières secondes...\n\n");
    }
  }
}
