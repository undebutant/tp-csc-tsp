#include <stdlib.h>
#include "list.h"

/* initialize a list */
void list_init(struct list_token*list) {
  list->next = NULL;
  list->prev = NULL;
  list->data = NULL;
}

/* insert token after list */
void list_insert_after(struct list_token*list, struct list_token*token) {
  token->next = list->next;
  token->prev = list;
  list->next = token;
  if(token->next) {
    /* the list was empty */
    token->next->prev = token;
  }
}

/* remove a token from the list */
void list_extract_token(struct list_token*token) {
  if(token->next) {
    token->next->prev = token->prev;
  }
  if(token->prev) {
    token->prev->next = token->next;
  }
}

/* add data to the list */
void list_add(struct list_token*list, void*data) {
  struct list_token* new_token = malloc(sizeof(struct list_token));
  new_token->data = data;
  list_insert_after(list, new_token);
}


/* remove token from the list and free its memory*/
void list_remove(struct list_token *token) {
  /* remove token from the list */
  list_extract_token(token);
  /* free the memory */
  free(token);
}


/* return the number of tokens in the list */
int list_nb_token(struct list_token*list) {
  int result = 0;
  struct list_token* cur_token = list->next;
  while(cur_token) {
    cur_token = cur_token->next;
    result++;
  }
  return result;
}
