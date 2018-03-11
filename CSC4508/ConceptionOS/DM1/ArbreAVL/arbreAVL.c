#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// Noeud de notre arbre AVL
struct Node
{
  char *key;
  char *value;
  struct Node *left;
  struct Node *right;
  int height;
};

// Fonction utilitaire renvoyant le maximum de deux entiers
int max(int a, int b)
{
  return (a > b)? a : b;
}

// Fonction utilitaire pour recuperer la hauteur d'un arbre
int height(struct Node *node)
{
  if (!node)
    return 0;
  return node->height;
}

// Procedure qui recalcule le champs height de node a partir de la
// hauteur de ses noeuds fils
void updateHeight(struct Node *node)
{
  // Gestion du cas ou le noeud envoye est NULL
  if (!node) {
    return;
  }
  // On se sert de height et de max definis avant pour en déduire
  // la valeur de height du noeud donne
  node->height = max(height(node->left), height(node->right)) + 1;
}

/* Definition des permutations (rotation a droite et rotation a gauche
 * afin de reequilibrer l'arbre binaire lors de l'insertion de nouveaux
 * noeuds. On fera attention a bien mettre a jour la hauteur des
 * noeuds bouges
 *
 * Cas de la permutation a droite
*/
struct Node *rightRotate(struct Node *z) {
  // On definit les noeuds y et T3 comme dans le schema exemple
  struct Node *nodeY = z->left;
  struct Node *nodeT3 = nodeY->right;

  // On procede a la rotation avec deux permutations de fils
  nodeY->right = z;
  z->left = nodeT3;

  // On met a jour la profondeur de l'arbre ainsi obtenu, en remarquant que
  // seules les hauteurs des noeuds Y et Z ont changees, et Y sera mis a jour
  // en sortie de cette fonction
  updateHeight(z);

  // On retourne la nouvelle racine du sous-arbre
  return nodeY;
}

// Cas de la permutation a gauche
struct Node *leftRotate(struct Node *z) {
  // On definit les noeuds y et T2 comme dans le schema exemple
  struct Node *nodeY = z->right;
  struct Node *nodeT2 = nodeY->left;

  // On procede a la rotation avec deux permutations de fils
  nodeY->left = z;
  z->right = nodeT2;

  // On met a jour la profondeur de l'arbre ainsi obtenu, en remarquant que
  // seules les hauteurs des noeuds Y et Z ont changees, et Y sera mis a jour
  // en sortie de cette fonction
  updateHeight(z);
  
  // On retourne la nouvelle racine du sous-arbre
  return nodeY;
}

/* Cree un noeud pour stocker la cle
   NB :
      - La cle est recopiee dans une zone memoire allouee dans newNode
      - Les pointeurs left et right sont a NULL
      - Le champ height est initialise a 1 (ce noeud est considere comme 
        une feuille autonome).
*/
struct Node* newNode(char *key, char *value)
{
  // Malloc sur la structure noeud et verification
  struct Node *nodeCreated = malloc(sizeof(struct Node));
  assert(nodeCreated);

  // Malloc pour key, en s'aidant de la chaine de caractere passee en
  // argument de la fonction
  nodeCreated->key = malloc(strlen(key) + 1);
  assert(nodeCreated->key);

  // Malloc pour value
  nodeCreated->value = malloc(strlen(value) + 1);
  assert(nodeCreated->value);

  // Copie propre des chaines de caractere dans la structure
  assert(strcpy(nodeCreated->key, key));
  assert(strcpy(nodeCreated->value, value));

  // Initialisation des pointeurs restants et de la hauteur
  nodeCreated->left = NULL;
  nodeCreated->right = NULL;

  nodeCreated->height = 1;

  return nodeCreated;
}

// Fonction recursive qui insere key dans le sous-arbre de racine node.
// Renvoie la nouvelle racine de ce sous-arbre.
// NB : un arbre binaire ne contient jamais deux fois la meme valeur.
//      De ce fait, si la valeur existe deja, insert renvoie node, sans creer
//      de nouveau noeud.
struct Node* insert(struct Node* node, char* key, char *value)
{
  // Si le noeud pointe est nul, on cree un nouveau noeud avec la key fournie
  if (!node) {
    node = newNode(key, value);
  }
  // Cas ou key < node->key (au sens de strcmp), on recurre a gauche
  else if (strcmp(key, node->key) < 0) {
    node->left = insert(node->left, key, value);
  }
  // Cas ou key > node->key, on recurre a droite
  else if (strcmp(key, node->key) > 0) {
    node->right = insert(node->right, key, value);
  }
  // Cas d'egalite entre les key, on remplace seulement value
  else {
    // La nouvelle value pouvant changer de taille il faut faire un realloc
    node->value = realloc(node->value, strlen(value) + 1);
    assert(node->value);

    // Puis on copie la nouvelle value dans le malloc redimensionne
    assert(strcpy(node->value, value));
  }

  // Calcul de la balance du noeud
  if (height(node->left) - height(node->right) > 1) {
    // Traitement du cas gauche gauche
    if (height(node->left->left) - height(node->left->right) > 0) {
      node = rightRotate(node);
    }
    // Traitement du cas gauche droite
    else {
      node->left = leftRotate(node->left);
      node = rightRotate(node);
    }
  }
  else if (height(node->left) - height(node->right) < -1) {
    // Traitement du cas droite droite
    if (height(node->right->left) - height(node->right->right) < 0) {
      node = leftRotate(node);
    }
    // Traitement du cas droite gauche
    else {
      node->right = rightRotate(node->right);
      node = leftRotate(node);
    }
  }

  // Mise a jour de la hauteur du noeud courant
  updateHeight(node);
  
  // Return servant dans tous les cas
  return node;
}

// Fonction de parcourt de l'arbre afin de renvoyer la value associee
// si trouvee, ou NULL sinon
char *get(struct Node *node, char *key) {
  // On s'assure que node n'est pas NULL
  if (node) {
    // Si on tombe sur la bonne key, on renvoie value
    if (!strcmp(node->key, key)) {
      return node->value;
    }
    // Si la comparaison montre que key < node->key est avant selon strcmp
    // alors, les contraintes de creation de l'arbre garantissent que key,
    // si elle existe, sera a gauche
    else if (strcmp(key, node->key) < 0 && node->left) {
      return get(node->left, key);
    }
    // On applique la meme logique que dans le commentaire precedent, avec
    // la relation > et en allant a droite
    else if (strcmp(key, node->key) > 0 && node->right) {
      return get(node->right, key);
    }
  }
  return NULL;
}

// Procedure affichant un certain nombre de tabulation
void print_tabs(int nb_tabs) {
  for(int i = 0; i<nb_tabs; i++) printf("  ");
}

// Procedure faisant des printf des cles et de la hauteur associes a
// chaque noeud du sous-arbre node.
void affiche(struct Node *node, int depth)
{
  if (node) {
    print_tabs(depth);
    printf("Hauteur %d : \"%s\"\n", node->height, node->key);

    print_tabs(depth);
    printf("left de \"%s\"\n", node->key);
    affiche(node->left, depth+1);

    print_tabs(depth);
    printf("right de \"%s\"\n", node->key);
    affiche(node->right, depth+1);
  }
}

// Fonction liberant tous les elements du sous-arbre node
// (y compris node)
// Renvoie systematiquement NULL
struct Node *release(struct Node *node)
{
  // On veut liberer les noeuds recursivement, on doit anticiper le cas
  // de base ou le noeud pointe est NULL
  if (node) {
    // On recurre sur les fils a gauche du noeud et on verifie qu'on obtient NULL
    struct Node *leftRel = release(node->left);
    assert(leftRel == NULL);
    node->left = NULL;
 
    // On recurre sur les fils a droite du noeud et on verifie qu'on obtient NULL
    struct Node *rightRel = release(node->right);
    assert(rightRel == NULL);
    node->right = NULL;
 
    // On libere le malloc sur node->key AVANT de liberer le malloc de node
    free(node->key);
    node->key = NULL;

    // On libere le malloc sur node->value AVANT de liberer le malloc de node
    free(node->value);
    node->value = NULL;
    
    // Liberation du malloc de node, et mise a NULL du pointeur
    free(node);
    node = NULL;
  }

  return node;
}

/* Code de test des fonctions ci-dessus */
#define MAX_LEN 128
struct Node *insertRaoul(struct Node *root) {
  char nom[MAX_LEN];
  strncpy(nom, "Raoul", MAX_LEN);
  return insert(root, nom, "le pitbull");
}

int main()
{
  struct Node *root = NULL;
  char *s = NULL;
  char nom[MAX_LEN];
 
  /* Construction d'un arbre */
  root = insert(root, "Leon", "la guenon");
  root = insert(root, "Hector", "l'alligator");

  root = insertRaoul(root);

  s = malloc(MAX_LEN);
  assert(s);
  strncpy(s, "Eric", MAX_LEN);
  root = insert(root, s, "le porc-epic");
  free(s);
  s = NULL;
  s = malloc(MAX_LEN);
  assert(s);
  strncpy(s, "Kelly", MAX_LEN);
  root = insert(root, s, "la souris");
  free(s);
  s = NULL;

  strncpy(nom, "Quentin", MAX_LEN);
  root = insert(root, nom, "le lamentin");
  strncpy(nom, "Daniella", MAX_LEN);
  root = insert(root, nom, "le chamois");

  root = insert(root, "Geraldine", "le cygne");
  root = insert(root, "Babette", "la chouette");

  /* Tests de la fonction get */
  printf("get(\"Leon\") renvoie \"%s\"\n", get(root, "Leon"));
  printf("get(\"Kelly\") renvoie \"%s\"\n", get(root, "Kelly"));
  printf("get(\"Inexistant\") renvoie \"%s\"\n", get(root, "Inexistant"));
  root = insert(root, "Leon", "le thon");
  printf("Apres MAJ, get(\"Leon\") renvoie \"%s\"\n", get(root, "Leon"));
  root = insert(root, "Leon", "le hanneton glouton de Tron (merci a Douglas Adams et Jean Bonnefoy pour ce chouette nom)");
  printf("Apres MAJ, get(\"Leon\") renvoie \"%s\"\n", get(root, "Leon")); 
  /* Affichage */
  printf("Arbre resultant \n");
  affiche(root, 0);

  /* Liberation */
  root = release(root);

  return 0;
}
