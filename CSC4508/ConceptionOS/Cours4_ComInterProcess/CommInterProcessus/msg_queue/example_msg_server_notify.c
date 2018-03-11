/*************************/
/* example_msg_server_v2.c */
/*************************/

/* Server process. Run it with
 * $ example_msg_server_v2 /message_queue_name &
 * Result : prints the clients' requests
 * Stops when a client sends a message starting with @
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>

#include "example_msg.h"

mqd_t requests, responses;
char key_req[80];
char key_rep[80];

void handle_request(union sigval sv);

void ask_for_notification() {
  struct sigevent sev;
  /* Calls the callback. We could also generate a signal and then intercept and process this signal */
  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_notify_function = handle_request;
  sev.sigev_notify_attributes = NULL;
  sev.sigev_value.sival_ptr = NULL;   /* Arg. to thread func. */
  if (mq_notify(requests, &sev) == -1) {
    perror("Server: mq_notify");
    exit(EXIT_FAILURE);
  }
}

void terminate_server(int retval) {
    printf("Terminating the server\n");

    /* close the message queues */
    if(mq_close(requests) < 0) {
      perror("Serveur : mq_close");
      exit(EXIT_FAILURE);
    }

    if(mq_close(responses) < 0) {
      perror("Serveur : mq_close");
      exit(EXIT_FAILURE);
    }

    /* delete the message queues */
    if(mq_unlink(key_req) < 0) {
      perror("Serveur : mq_unlink");
      exit(EXIT_FAILURE);
    }

    if(mq_unlink(key_rep) < 0) {
      perror("Serveur : mq_unlink");
      exit(EXIT_FAILURE);
    }

    exit(retval);
}

void handle_request(union sigval sv) {
  struct msg req, rep;
  int i;
  int len;

  len = mq_receive(requests, (char*)&req, sizeof(req), NULL);
  if(len < 0) {
    perror("Server: mq_receive");
    exit(EXIT_FAILURE);
  }

  printf("Server: received message: %s \n", req.text);

  if (req.text[0] == '@') {
    /* ask to terminate the server */

    /* reply the client */
    strcpy(rep.text, "Terminating the server");

    if( mq_send(responses, (char*)&rep, GET_SIZE(&rep), 0) < 0) {
      perror("Serveur : mq_send");
      exit(EXIT_FAILURE);
    }

    /* terminate the server */
    terminate_server(EXIT_SUCCESS);
  }


  /* process the request */
  for (i = 0; req.text[i] != 0; i++) {
    rep.text[i] = req.text[i];
  }
  rep.text[i] = '\0';


  /* send the response to the client */
  if (mq_send(responses, (char*)&rep, GET_SIZE(&rep), 0) < 0) {
    perror("Server: mq_send");
    exit(EXIT_FAILURE);
  }
  printf("Server: message sent: %s \n", rep.text);

  /* set up notifications */
  ask_for_notification();
}


int main(int argc, char** argv) {

  if(argc != 2) {
    fprintf(stderr, "Usage: %s /message_queue_name\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  sprintf(key_req, "%s_req", argv[1]);
  sprintf(key_rep, "%s_rep", argv[1]);

  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;
  attr.mq_curmsgs = 0;
  attr.mq_msgsize = sizeof(struct msg);

  /* message queue for incoming requests */
  requests = mq_open(key_req, O_CREAT|O_RDWR, S_IRWXU, &attr);
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


  /* set up notifications */
  ask_for_notification();

  for (;;) {
    sleep(1);
  }


  sleep(10);

  return EXIT_SUCCESS;
}
