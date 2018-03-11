/* 
 * Programme simulant la gestion d'une imprimante
 */

// Penser a -lrt lors de la compilation

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>

struct msg {
  char text[256];
};

#define GET_SIZE(msg) (strlen((msg)->text)+sizeof('\0'))

void attenteImpression(mqd_t requests){
  struct msg req;
  int len;

  do {
    printf("Nom du fichier a imprimer (taper '0' pour terminer) ? ");
    scanf("%s", req.text);
    if (strcmp(req.text,"0") == 0){
      break;
    }
    /* send the request */
    len = mq_send(requests, (char*)&req, GET_SIZE(&req), 1);
    if (len < 0) {
      perror("Client: mq_send");
      exit(EXIT_FAILURE);
    }
  }while (1);
}

int main(){
  mqd_t requests;
  char key_req[80];

  struct mq_attr attr;

  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;                          /* size of the message queue */
  attr.mq_curmsgs = 0;
  attr.mq_msgsize = sizeof(struct msg);         /* size of one message */

  /* message queue for sending requests */
  requests = mq_open("/yolo", O_WRONLY);
  if (requests < 0) {
    perror("Client: mq_open");
    exit(EXIT_FAILURE);
  }

  attenteImpression(requests);
  return EXIT_SUCCESS;
}
