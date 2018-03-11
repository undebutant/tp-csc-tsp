/*
 * A simple Multiplexing C socket server with poll()
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>   //write
#include <sys/time.h>
#include <time.h>
#include <poll.h>

#define MAXFDS		100
#define STDIN  		0
#define MYMSGLEN 	2048
#define COSTCTS    	23
#define INTERVALSEC  	10
  
  
  
  int costCTS; //Variable used to store the cost up to date

/*------------------------------------------------------------------------------*/  
 int process_socket_fds_read(int client_sock)
 {
   
  int read_size;
  char client_message[MYMSGLEN];

  struct messageCost{
    uint32_t cost;
    uint32_t msgLen;
    char buff[MYMSGLEN]; 
  } msg;
  
  memset(client_message,0,MYMSGLEN);
  if(client_sock==STDIN)
  {
	
	scanf("%s",client_message);
	printf("\n----------------------------------------------------------------------\n");
	printf("Reading from Standard Input (Stdin): %s\n", client_message);
	printf("----------------------------------------------------------------------\n");
	if(!memcmp("quit#",client_message,5))
	{
	 printf("Shuting down the server \n");
	 return (-2);
	}
	if(!memcmp("*C*",client_message,3))
	{
	  if(1!=sscanf(client_message,"*C*%d",&costCTS))
	  {
	    printf("\n----------------------------------------------------------------------\n");
	    printf("Could not get the value, please try again\n");
	    printf("----------------------------------------------------------------------\n");
	    return(1);
	    }
	    printf("\n----------------------------------------------------------------------\n");
	  printf("The new Cost is %d\n",costCTS);
	  printf("----------------------------------------------------------------------\n");
	}
	else
	{
	  printf("\n----------------------------------------------------------------------\n");
	 printf("Bad Format! Format expected *C*XXX\n"); 
	 printf("----------------------------------------------------------------------\n");
	}
	return(1);
  }
  if((read_size=recv(client_sock,client_message,MYMSGLEN,0))>0)
    {
	printf("\n----------------------------------------------------------------------\n");
      printf("Msg received from socket descriptor (%d): %s, size of the message received: %d \n",client_sock,client_message,read_size);
      printf("----------------------------------------------------------------------\n");
      
      msg.cost=htonl((uint32_t) (costCTS * read_size));
    
      msg.msgLen=htonl((uint32_t)read_size);
      memcpy(msg.buff,client_message,read_size);
      send(client_sock,(char*)&msg, read_size+sizeof(uint32_t)+sizeof(uint32_t),0);         
      return 1;
    }

  if(read_size==0)
  {
    puts("Client disconnected");
    fflush(stdout);
  }
  else  //(read_size== -1)
  {
    perror("Recv failed");
  } 
	
  
  close(client_sock);
  
  return -1;
}

/*------------------------------------------------------------------------------*/
int closing_all_sockets(struct pollfd ufds[],int nbfds)
{
  int i;
  
  for(i=0;i<nbfds;i++)
  {
      if(ufds[i].fd>0)
	  close(ufds[i].fd);
      //Remember that we do not close the STDIN neither a negative descriptor
  }
  return 0;
}
/*------------------------------------------------------------------------------*/
int squeezing_ufds(struct pollfd ufds[],int nbfds)
{
 int i,j;
 
 
  for(i=0;i<nbfds;i++)
  {
      if(ufds[i].fd==-1)	
      {	
	for(j=i;j<nbfds;j++)
	    ufds[j].fd=ufds[j+1].fd;
	nbfds--;
      }	
  }
  return nbfds;
}
/*------------------------------------------------------------------------------*/
int timeToTimeout(struct timeval *current_time,struct timeval *nextTimeout)
{
  int interval;
  struct timeval interval_time;
  
  if (timercmp(current_time, nextTimeout, >))
  {
    //interval_time.tv_sec=INTERVALSEC;
    //interval_time.tv_usec=0;
    //timeradd(nextTimeout,&interval_time,nextTimeout);   
    //interval=INTERVALSEC*1000;
    nextTimeout->tv_sec=current_time->tv_sec;
    nextTimeout->tv_usec=current_time->tv_usec;
    interval=0; // doing this way we impose to the next poll to return immediately as a timeout expiration
  }
    
 else
    {
      timersub(nextTimeout,current_time,&interval_time);
      interval=interval_time.tv_sec *1000;
      
    }
 return interval; 
}

/*------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{

  int socket_desc, client_sock; 
  int socket_size;
  int ret, ret_code;
  
  struct sockaddr_in server, client;
  
  // file descriptors set used by poll  
  struct pollfd ufds[MAXFDS];

  
  struct timeval  current_time,interval_time, nextTimeout;
  int interval;
  
  int i;
  int nbfds=0;
  int squeeze;
  
  
  //variables for setsockopt
  int code, enable=1;
  
  costCTS=COSTCTS;
 
  socket_size=sizeof(struct sockaddr_in);
  
//Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1)
  {
      printf("Could not create socket\n");
      return -1;
  }

  puts("Socket created");
  
  
//Prepare the sockaddr_in structure

  server.sin_family=AF_INET;
  server.sin_addr.s_addr=INADDR_ANY;
  server.sin_port = htons(8888);

//Reuse the same port 

  code=setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR,&enable, sizeof(int));
  if (code == -1)
  {
    perror("setsockopt");
    exit(1);
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
  if(listen(socket_desc,5)<0)
  {
     //print the error message
      perror("Error on listen:");
      close(socket_desc);
      return -1;
  }
  puts("Listen call done");

//Initialize the pollfd structre
  memset(ufds,0,sizeof(ufds));
   
/* Add stdin to the descriptor set */
  ufds[0].fd=0;
  ufds[0].events=POLLIN;
  nbfds ++;
/* Add the socket listener to the master set */
  ufds[1].fd=socket_desc;
  ufds[1].events=POLLIN;
  nbfds++; 


//Preparing for the polling 

  gettimeofday(&current_time,NULL);
  interval_time.tv_sec=INTERVALSEC;
  interval_time.tv_usec=0;
  timeradd(&current_time,&interval_time,&nextTimeout);
 
  printf("Starting at Server at: %s\n",ctime((&current_time.tv_sec)));
  printf("To update the cost type *C* followed by the new value\n");
  printf("Waiting for incoming connections....\n");
  
  while(1)
{
    
  interval=timeToTimeout(&current_time,&nextTimeout);
  //Poll call waiting for ready  descriptors
  ret=poll(ufds,nbfds,interval);
	
  //On return select()  may  update  the timeout argument to indicate how much time was left
  //It is the case in our system otherwise we should proceed in some otherway to readjust the interval value accordingly
  //Another assumption is that our processing is short enough to keep the accuracy
  
  if(ret==-1)
  {
    perror("Error on poll");
    closing_all_sockets(ufds,nbfds);  // do not try to close STDIN
    return -1;
  }
  
  /*looking through opened socket file descriptors ready to be read*/
   
  if (ret==0) //timeout expired
  {
     gettimeofday(&current_time,NULL);
     printf("\n----------------------------------------------------------------------\n");
      printf("======> %d more seconds have elapsed : %s\n",INTERVALSEC,ctime((&current_time.tv_sec)));
     printf("----------------------------------------------------------------------\n");
      
	timeradd(&nextTimeout, &interval_time,&nextTimeout);
    continue;  
  }
  squeeze=0;
  
 
  for(i=0;i<nbfds; i++)
  {
    
    if(ufds[i].revents & POLLIN)
    {
      if(ufds[i].fd==socket_desc)
      {
	//Accepting a new connection
	if((client_sock = accept(socket_desc, (struct sockaddr *) &client,(socklen_t*) &socket_size))<0)
	{
	  perror("Server accept problem, closing all sockets:");
	  closing_all_sockets(ufds,nbfds);
	  return -1;
	}
	else
	{
	  if(nbfds<MAXFDS)
	  {
	    printf("Server accepting new connection\n");  
	    ufds[nbfds].fd=client_sock;
	    ufds[nbfds].events=POLLIN;
	    ufds[nbfds].revents=0;
	    nbfds++;
	  }
	  else
	  {
	    printf("Server full rejecting the incoming connection request\n");
	    close(client_sock);
	  }
	}
      }
      else
      {
	//a socket ready to be read, data available, read and process it
	ret_code=process_socket_fds_read(ufds[i].fd);
	if(ret_code==-1)
	{//connection closed, unset the corresponding descriptor
	  close(ufds[i].fd);
	//squeezing the ufds will be done after the For loop, for now the fd is simply put to -1
	  ufds[i].fd=-1;
	  squeeze=1;
	}
	if(ret_code==-2)
	{
	 //Closing all attached sockets and exiting, do not close STDIN
	 closing_all_sockets(ufds,nbfds);
	  return -1;
	}
      }
    }
  }
  if(squeeze)
  {
    nbfds=squeezing_ufds(ufds,nbfds);	
    squeeze=0;
  }
 gettimeofday(&current_time,NULL); 
}
  

}//end of main 
