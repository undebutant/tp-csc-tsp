/*
 * A simple Multiplexing C socket server with select()
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>   //write
#include <sys/time.h>
#include <time.h>


#define STDIN  0
#define MYMSGLEN 2048
#define COSTCTS    23
#define INTERVALSEC  10
  
  
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
	printf("\n----------------------------------------------------------------------\n");
	scanf("%s",client_message);
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
    puts("client disconnected");
    fflush(stdout);
  }
  else  //(read_size== -1)
  {
    perror("recv failed");
  } 
	
  
  close(client_sock);
  
  return -1;
}

/*------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{

  int socket_desc, client_sock; 
  int socket_size;
  int fdmax, fdmaxTmp;
  int ret, ret_code;
  int code, enable=1;
  
  struct sockaddr_in server, client;
  
  // file descriptors set used by select
  fd_set read_fds;
  fd_set read_fds_master;
  
  int i,j;
  
  struct timeval interval, current_time;
  
  socket_size=sizeof(struct sockaddr_in);
  
  costCTS=COSTCTS;
  
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
    perror("Setsockopt");
    exit(1);
  }
  puts("Server-setsockopt() is OK...");
  
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
  
//Initialize fd_set
  FD_ZERO(&read_fds); //set to 0
  FD_ZERO(&read_fds_master);
 
  /* add the socket listener to the master set */

FD_SET(socket_desc, &read_fds_master);

/* add stdin to the master set */

FD_SET(STDIN,&read_fds_master);

//Preparing for the select call
//keep track of the biggest file descriptors
fdmax=socket_desc; //for now it is the only descriptors
  
  gettimeofday(&current_time,NULL);
  interval.tv_sec=INTERVALSEC;
  interval.tv_usec=0;
 
  printf("Starting at Server at: %s\n",ctime((&current_time.tv_sec)));
  printf("To update the cost type *C* followed by the new value\n");
 printf("Waiting for incoming connections....\n");  
  while(1)
{
  fdmaxTmp=fdmax;
  //printf("fmax is equal to %d \n",fdmax);
  
  read_fds=read_fds_master;

  //Select call waiting for ready  descriptors
  ret=select(fdmax+1,&read_fds,NULL,NULL,&interval);
  
  //On return select()  may  update  the timeout argument to indicate how much time was left
  //It is the case in our system otherwise we should proceed in some otherway to readjust the interval value accordingly
  //Another assumption is that our processing is short enough to keep the accuracy
  
  if(ret==-1)
  {
    perror("Error on server select");
    for(j=3;j<=fdmaxTmp; j++)
	     {
	      if(FD_ISSET(j,&read_fds_master))
	      close(j);
	     }
    return -1;
  }
  
  /*looking through opened socket file descriptors ready to be read*/
  
  if (ret==0) //timeout expired
  {
    gettimeofday(&current_time,NULL);	
    printf("%d more seconds have elapsed : %s\n",INTERVALSEC,ctime((&current_time.tv_sec)));
      interval.tv_sec=INTERVALSEC;
      interval.tv_usec=0;
  
	
    continue;  
  }
  
  for(i=0;i<=fdmaxTmp; i++)
  {
    if(FD_ISSET(i,&read_fds))
    {
      if(i==socket_desc)
      {
	//handle_new_connection
	if((client_sock = accept(socket_desc, (struct sockaddr *) &client,(socklen_t*) &socket_size))<0)
	{
	  perror("Server accept problem closing all sockets");
	  for(j=3;j<=fdmaxTmp; j++)
	     {
	      if(FD_ISSET(j,&read_fds_master))
	      close(j);
	     }
	  return -1;
	}
	else
	{
	  printf("Server accepting new connection\n");
	  FD_SET(client_sock,&read_fds_master); //adding the file descriptor to the master
	  if(client_sock>fdmax)
	    fdmax=client_sock; 
	  
	}
      }
      else
      {
	//a socket ready to be read
	//data available, read and process it
	ret_code=process_socket_fds_read(i);
	if(ret_code==-1)
	{
	
	//connection closed, unset the corresponding descriptor
	  
	  FD_CLR(i,&read_fds_master);
	  close(i);
	//find the new fdmax
	  for(j=fdmax;j>=0; j--)
	  {
	    if(FD_ISSET(j,&read_fds_master))
	      break;
	  }
	  fdmax=j;
	  
	}
	if(ret_code==-2)
	{
	 //Closing all attached sockets and exiting, do not close STDIN
	  for(j=3;j<=fdmax; j++)
	  {
	    if(FD_ISSET(j,&read_fds_master))
	      close(j);
	    
	  }
	  return -1;
	}
      }
    }
  }

}


  

}
