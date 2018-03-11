#ifndef LIST_H
#define LIST_H

struct list_token {
  /* pointer to the next token */
  struct list_token *next;

  /* pointer to the previous token */
  struct list_token *prev;

  /* pointer to the data */
  void* data;
};

/* initialize a list */
void list_init(struct list_token*list);

/* add data to the list */
void list_add(struct list_token*list, void*data);

/* insert (an already allocated) token after list */
void list_insert_after(struct list_token*list, struct list_token*token);

/* remove a token from the list */
void list_extract_token(struct list_token*token);

/* remove token from the list and free it*/
void list_remove(struct list_token *token);

/* return the number of tokens in the list */
int list_nb_token(struct list_token*list);



#endif
