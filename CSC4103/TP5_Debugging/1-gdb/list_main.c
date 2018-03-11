#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "list.h"

/* The goal of this program is to create a list of (random) int value,
 * and to sort them.
 */


/* The head of the list.
 * It's just a token whose next field points to the
 * first token of the list.
 */
struct list_token list;


/* insert v in the list */
void add_value(int v) {
  int*value = malloc(sizeof(int));
  *value = v;
  list_add(&list, value);
}

/* print the content of the list */
void print_list() {
  printf("The list contains:\n");

  /* for each token of the list */
  struct list_token* cur_pos = list.next;
  while(cur_pos) {
    /* get the int value */
    int* v = cur_pos->data;
    printf("%d  ", *v);
    /* jump to the next token */
    cur_pos = cur_pos->next;
  }
  printf("\n");
}

/* print the content of the list in reverse order */
void print_list_reverse() {
  struct list_token* cur_pos = &list;
  
  /* find the last token */
  while(cur_pos->next) {
    cur_pos = cur_pos->next;
  }
  /* now, cur_pos points to the last token */

  /* print the list starting from the last token */
  printf("The reverse list contains:\n");
  while(cur_pos->prev) {
    /* get the int value */
    int* v = cur_pos->data;
    printf("%d  ", *v);
    /* jump to the previous token */
    cur_pos = cur_pos->prev;
  }
  printf("\n");
}


/* find the token with the minimum value starting from first_token */
struct list_token* find_min(struct list_token* first_token) {
  struct list_token* cur = first_token;
  struct list_token* min = first_token;
  int* min_val_ptr = cur->data;
  int  min_val = *min_val_ptr;

  /* for each token of the list */
  while(cur) {
    int *value_ptr = cur->data;
    /* compare with the min value */
    if(*value_ptr < min_val) {
      min_val = *value_ptr;
      min = cur;
    }
    /* jump to the next token */
    cur = cur->next;
  }
  return min;
}

/* insert 'to_insert' before 'first' */
void insert_before(struct list_token*to_insert, struct list_token*first) {

  if(first->prev) {
    /* there's a token before first. Let's insert after this token */
    list_insert_after(first->prev, to_insert);
  } else {
    /* first is the first token of the list. Let's insert at the begining of the list */
    list_insert_after(&list, to_insert);
  }
}

/* sort the list starting from first_token */
void sort_list(struct list_token*first_token) {
  if(!first_token) {
    /* the list is empty */
    return;
  }

  /* find the min value */
  struct list_token*min = find_min(first_token);

  if(min != first_token) {
    /* insert the min value at the beginning */
    insert_before(min, first_token);
  }

  /* sort the remaining of the list */
  sort_list(min->next);
}


int main(int argc, char** argv) {
  srand(time(NULL));
  list_init(&list);

  printf("Generating the list...\n");
  int i;
  for(i=0; i<10; i++) {
    add_value(rand()%100);
  }

  print_list();
  print_list_reverse();

  printf("\n\nSearching for the min value...\n");
  struct list_token* min_token = find_min(list.next);
  int* min = min_token->data;
  printf("min=%d\n", *min);

  printf("\n\nSorting the list...\n");
  sort_list(list.next);
  printf("Sorted list:\n");
  print_list();
  return EXIT_SUCCESS;
}
