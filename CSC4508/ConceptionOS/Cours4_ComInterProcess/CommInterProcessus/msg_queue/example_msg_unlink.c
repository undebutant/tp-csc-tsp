/*************************/
/* example_msg_unlink.c */
/*************************/

/* Cleaning process. Remove an existing message queue
 * Run it with
 * $ example_msg_unlink /message_queue_name
 */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>

int main(int argc, char** argv) {
  char* key_req, *key_rep;

  if(argc != 2) {
    fprintf(stderr, "Usage: %s message_queue_name\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  asprintf(&key_req, "%s_req", argv[1]);
  asprintf(&key_rep, "%s_rep", argv[1]);

  if(mq_unlink(key_req) < 0) {
    perror("Serveur : mq_unlink");
    exit(EXIT_FAILURE);
  }

  if(mq_unlink(key_rep) < 0) {
    perror("Serveur : mq_unlink");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
