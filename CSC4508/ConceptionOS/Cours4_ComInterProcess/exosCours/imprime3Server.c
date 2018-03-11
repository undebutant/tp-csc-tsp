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


void impression(char *nomFic){

  printf("Debut d'impression de \"%s\"...\n", nomFic);
  sleep(5); /* Simule la lenteur de l'impression */
  printf("...Fin d'impression de \"%s\"...\n", nomFic);

}

void attenteImpression(mqd_t requests){
  struct msg req;

  do {
    /* wait for any requests */
    int len = mq_receive(requests, (char*)&req, sizeof(req), NULL);
    if(len < 0) {
      perror("Server: mq_receive");
      exit(EXIT_FAILURE);
    }

    printf("Server: received message: %s \n", req.text);

    if (req.text[0] == '0') {
      /* ask to terminate the server */
      break;
    }
    impression(req.text);
  }while (1);
}

int main(){

  mqd_t requests;
  char key_req[80];

  struct mq_attr attr;

  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;				/* size of the message queue */
  attr.mq_curmsgs = 0;
  attr.mq_msgsize = sizeof(struct msg);		/* size of one message */

  /* message queue for incoming requests */
  requests = mq_open("/yolo", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR, &attr);
  if (requests < 0) {
    perror("Server: mq_open");
    exit(EXIT_FAILURE);
  }

  attenteImpression(requests);
  return EXIT_SUCCESS;
}
