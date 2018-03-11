/************************/
/* example_msg_client.c */
/************************/

/* Client process. Run it with
 * $ example_msg_client /message_queue_name text_to_send
 * Result: print the message received from the server
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include "example_msg.h"

int main(int argc, char **argv) {
  mqd_t requests, responses;
  char key_req[80];
  char key_rep[80];

  struct msg rep, req;
  int len;

  if (argc != 3) {
    printf("Client: Usage : %s /message_queue_name text_to_send\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  sprintf(key_req, "%s_req", argv[1]);
  sprintf(key_rep, "%s_rep", argv[1]);

  /* message queue for sending requests */
  requests = mq_open(key_req, O_WRONLY);
  if (requests < 0) {
    perror("Client: mq_open");
    exit(EXIT_FAILURE);
  }

  /* message queue for receiving responses */
  responses = mq_open(key_rep, O_RDONLY);
  if (requests < 0) {
    perror("Client: mq_open");
    exit(EXIT_FAILURE);
  }

  /* prepare the request to send */
  strcpy(req.text, argv[2]);
  /* send the request */
  len = mq_send(requests, (char*)&req, GET_SIZE(&req), 1);
  if (len < 0) {
    perror("Client: mq_send");
    exit(EXIT_FAILURE);
  }
  printf("Client: message sent: %s \n", req.text);

  /* Wait for a responses */
  len = mq_receive(responses, (char*)&rep, sizeof(rep), NULL);
  if(len < 0) {
    perror("Client: mq_receive");
    exit(EXIT_FAILURE);
  }
  printf("Client: message received: %s \n", rep.text);

  /* close the message queues */
  if((mq_close(requests) < 0) || (mq_close(responses) < 0)){
    perror("Serveur : mq_close");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
