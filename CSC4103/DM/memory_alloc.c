#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "memory_alloc.h"


/* the memory allocator */
struct memory_alloc_t m;


/* Initialize the memory_alloc_t structure */
void memory_init() {
  /* On part pour l'instant du principe que la mémoire fait la taille DEAFULT_SIZE */
  m.available_blocks = (size_t) DEFAULT_SIZE;
  
  /* On initialise à 0 les variables premier bloc et erreur */
  m.first_block = 0;
  m.error_no = 0;
  
  /* On remplit le tableau représentant la mémoire... */
  for (int i = 0 ; i < DEFAULT_SIZE ; i++) {
    if (i == DEFAULT_SIZE - 1) {
      /* ... En se rappelant que la dernière case vaut -1 */
      m.blocks[i] = -1;
    }
    else {
      m.blocks[i] = i + 1;
    }
  }
}


/* return the number of consecutive blocks starting from first */
int nb_consecutive_blocks(int first) {
  int pos_actuelle = first;
  int decompte_libre = 1;
  
  /* On gère le cas particulier ou la mémoire est pleine en créant une erreur et retournant -1 */
  if (m.first_block == -1) {
    m.error_no = E_MEM_FULL;
    return -1;
  }
  
  /* Par défaut on s'arrête lorsqu'on atteint la valeur -1 du tableau... */
  while (m.blocks[pos_actuelle] != -1) {
    /*
     * ... Tant que on vérifie la condition d'avancement : les blocs sont consécutifs à
     * partir du premier
     */
    if (pos_actuelle + 1 == m.blocks[pos_actuelle]) {
      decompte_libre ++;
      pos_actuelle = m.blocks[pos_actuelle];
    }
    else {
      return decompte_libre;
    }
  }

  return decompte_libre;
}


void memory_reorder() {
  
  /* Gestion du cas de mémoire pleine */
  if (m.first_block == -1) {
    abort();
  }

  /* On récupère toutes les cases libres en suivant la chaîne jusqu'à -1 */
  int tab_tri_bulle[m.available_blocks + 1];
  int pos_actuelle = m.first_block;
  tab_tri_bulle[0] = pos_actuelle;

  for (int i = 1 ; i < m.available_blocks + 1 ; i++) {
    pos_actuelle = m.blocks[pos_actuelle];
    tab_tri_bulle[i] = pos_actuelle;
  }

  /* Ensuite on trie le tableau obtenu avec le trie par bulle */
  int temp_var = -2;

  for (int j = 0 ; j < m.available_blocks ; j++) {
    for (int k = 0 ; k < m.available_blocks - 1 ; k++) {
      if (tab_tri_bulle[k] > tab_tri_bulle[k+1]) {
        temp_var = tab_tri_bulle[k];
	tab_tri_bulle[k] = tab_tri_bulle[k+1];
	tab_tri_bulle[k+1] = temp_var;
      }
    }
  }

  /* Enfin, on défragmente la mémoire grâce à la liste triée */
  for (int x = 0 ; x < m.available_blocks ; x++) {
    m.blocks[ tab_tri_bulle[x] ] = tab_tri_bulle[x + 1];
  }

  m.first_block = tab_tri_bulle[0];
}


/* Allocate size consecutive bytes and return the index of the first
 * memory block
 */
int memory_allocate(size_t size) {
  
  /* On initialise quelqueqs variables pour connaître les blocs consécutifs, le point de départ,
   * et savoir ou on en est dans l'allocation */
  int available_consec_size = nb_consecutive_blocks(m.first_block);
  int start_case = m.first_block;
  int pos_actuelle = start_case;
  int temp_pos_next = start_case;
  
  /* Gestion du cas mémoire pleine */
  if (m.first_block == -1) {
    m.error_no = E_MEM_FULL;
    return -1;
  }
  /* Gestion du cas ou on a pas assez de mémoire consécutive */
  else if (8 * available_consec_size < size) {
    memory_reorder();
    if (8 * available_consec_size < size) {	  
      m.error_no = E_NOMEM;
      return -1;
    }
  }
  /* Gestion du cas allocation possible, on en profite pour la faire 
   * Pour l'exemple, on remplit avec la valeur - n° case de départ - 10 */
  int var_fill = - m.first_block - 10;
  for (int i = 0 ; (i * sizeof(memory_page_t)) < size ; i++) {
    /* Stockage de la case suivante dans le temp, fill de l'actuelle, et position pour l'itération suivante */
    temp_pos_next = m.blocks[pos_actuelle];
    m.blocks[pos_actuelle] = var_fill;
    pos_actuelle = temp_pos_next;
    /* On garde à jour le décompte des blocs encore libre */
    m.available_blocks --;
  }
    
  /* On met à jour le nouveau premier bloc libre */
  m.first_block = pos_actuelle;
  
  /* On renvoit le n° de la case de départ ou commence la mémoire attribuée */
  return start_case;
}


/* Free the size bytes memory space starting at address addr */
void memory_free(int address, size_t size) {
  
  /* On définit la position actuel et un décompte du nombre d'itération du for à venir */
  int pos_actuelle = address;
  int val_iterateur = 0;

  if (size != 8) {
    /* On parcourt le tableau à partir de l'address fournie, sur la plage indiquée, -1... */
    for (int i = 0 ; (i * sizeof(memory_page_t)) < size - 1 ; i++) {
      /* ...Ainsi on remplit les cases libérées avec leurs adresses respectives + 1 */
      m.blocks[address + i] = pos_actuelle + 1;
      /* On met à jour les variables et la structure */
      m.available_blocks ++;
      pos_actuelle ++;
      val_iterateur ++;
    }
  }
  
  /* On gère le cas particulier de la dernière case à libérer, qui aura pour successeur l'ancien premier block libre */
  m.blocks [address + val_iterateur] = m.first_block;
  
  /* On finit de mettre à jour la structure */
  m.available_blocks ++;
  m.first_block = address;
}


/* Print the current status of the memory_alloc_t structure */
void memory_print() {
  printf("---------------------------------\n");
  printf("\tBlock size: %lu\n", sizeof(m.blocks[0]));
  printf("\tAvailable blocks: %lu\n", m.available_blocks);
  printf("\tFirst free: %d\n", m.first_block);
  printf("\tStatus: %x\n", m.error_no);
  printf("\tContent:\n");
  int i;
  for(i=0; i<DEFAULT_SIZE; i++) {
    printf("%ld  ", m.blocks[i]);
  }
  printf("\n");
  printf("---------------------------------\n\n");
}


/* Print a message corresponding to errno */
void memory_error_print(enum memory_errno error_number) {
  switch(error_number) {
  case E_SUCCESS:
    printf("Success\n");
    break;
  case E_NOMEM:
    printf("Not enough memory\n");
    break;
  case E_SHOULD_PACK:
    printf("Not enough contiguous blocks\n");
    break;
  case E_MEM_FULL:
    printf("The memory is full\n");
    break;
  default:
    break;
  }
}


/* allocate size bytes and write value in the allocated buffer */
void test_alloc(size_t size, int value) {
  if(value >= -1) {
    /* make sure the value to insert is different from the values in the linked list */
    fprintf(stderr, "please insert a value < -1\n");
    abort();
  }

  /* allocate memory */
  int i;
  int res = memory_allocate(size);
  if(res < 0) {
    /* memory_allocate failed */
    fprintf(stderr, "Failed to allocate %lu bytes\n", size);
    memory_error_print(m.error_no);
    abort();
  }

  /* fill the allocated buffer with value */
  printf("memory_allocate(%lu) -> %d\n", size, res);
  for(i=0; (i * sizeof (memory_page_t)) < size ; i++) {
    m.blocks[i+res] = value;
  }

  /* print the memory blocks */
  memory_print();
}


/* deallocate size bytes starting from address */
void test_free(int address, size_t size) {
  if(address < 0 || address > DEFAULT_SIZE - 1) {
    /* make sure the value of address is positive and before DEFAULT_SIZE */
    fprintf(stderr, "Please insert a correct address < -1\n");
    abort();
  }

  /* deallocate memory */
  memory_free(address, size);

  /* print the memory blocks */
  printf("memory_free(%lu) starting at %d\n", size, address);
  memory_print();
}


int main(int argc, char**argv) {
  /* Test des différentes fonctions implémentées */
  memory_init();
  size_t i;
  memory_print();

  int next_value = -10;
  for(i=1; i<=64; i*=4) {
    test_alloc(i, next_value);
    next_value--;
  }

  test_alloc(24, next_value--);
  test_alloc(8, next_value--);
 
  test_free(12, 24);
  
  test_free(3,8);
  test_free(4,8);
  test_free(2,8);

  printf("nb_consecutive_blocks(0) returned %d\n\n", nb_consecutive_blocks(m.first_block));

  printf("memory_reorder()\n");
  memory_reorder();
  memory_print();
  printf("nb_consecutive_blocks(0) returned %d\n\n", nb_consecutive_blocks(m.first_block));

  return EXIT_SUCCESS;
}
