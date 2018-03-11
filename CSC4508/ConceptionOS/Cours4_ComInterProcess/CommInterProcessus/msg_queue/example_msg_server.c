/*************************/
/* example_msg_server.c */
/*************************/

/* Server process. Run it with
 * $ example_msg_server /message_queue_name &
 * Result : prints the clients' requests
 * Stops when a client sends a message starting with @
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>

#include "example_msg.h"

int main(int argc, char** argv) {
  mqd_t requests, responses;
  char key_req[80];
  char key_rep[80];

  struct msg req, rep;

  if(argc != 2) {
    fprintf(stderr, "Usage: %s /message_queue_name\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  sprintf(key_req, "%s_req", argv[1]);
  sprintf(key_rep, "%s_rep", argv[1]);

  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;		/* size of the message queue */
  attr.mq_curmsgs = 0;
  attr.mq_msgsize = sizeof(struct msg); /* size of one message */

  /* message queue for incoming requests */
  requests = mq_open(key_req, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR, &attr);
  if (requests < 0) {
    perror("Server: mq_open");
    exit(EXIT_FAILURE);
  }

  /* message queue for outgoing responses */
  responses = mq_open(key_rep, O_CREAT|O_RDWR, S_IRWXU, &attr);
  if (responses < 0) {
    perror("Server: mq_open");
    exit(EXIT_FAILURE);
  }

  for (;;) {
    /* wait for any requests */
    int len = mq_receive(requests, (char*)&req, sizeof(req), NULL);
    if(len < 0) {
      perror("Server: mq_receive");
      exit(EXIT_FAILURE);
    }

    printf("Server: received message: %s \n", req.text);

    if (req.text[0] == '@') {
      /* ask to terminate the server */
      break;
    }

    /* process the request */
    strcpy(rep.text, req.text);

    /* send the response to the client */
    len = mq_send(responses, (char*)&rep, GET_SIZE(&rep), 0);
    if (len < 0) {
      perror("Server: mq_send");
      exit(EXIT_FAILURE);
    }
    printf("Server: message sent: %s \n", rep.text);
  }

  strcpy(rep.text, "Terminating the server");

  if( mq_send(responses, (char*)&rep, GET_SIZE(&rep), 0) < 0) {
    perror("Serveur : mq_send");
    exit(EXIT_FAILURE);
  }

  /* wait to make sure that client received the server's answer */
  sleep(2);

  /* close the message queues */
  if( (mq_close(requests) < 0) || (mq_close(responses) < 0)) {
    perror("Serveur : mq_close");
    exit(EXIT_FAILURE);
  }

  /* delete the message queues */
  if((mq_unlink(key_req) < 0) || (mq_unlink(key_rep) < 0)) {
    perror("Serveur : mq_unlink");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
