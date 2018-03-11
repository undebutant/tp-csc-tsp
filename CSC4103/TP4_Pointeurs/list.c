#include <string.h>
#include <stdio.h>
#include <stdlib.h>



struct node {
	struct node * next;
	char element[256];
};


struct node* create (char * string) {
	
	struct node* noeud = malloc(sizeof(struct node));

	noeud->next = NULL;
	strcpy (noeud->element, string);
	
	return noeud;
}


void display (struct node* noeud) {
	
	printf("%s", noeud->element);

	if (noeud->next != NULL) {
		display(noeud->next);
	}
	else {
		printf("\n\n");
	}
}


void freeNodes (struct node* noeud) {
	if (noeud->next != NULL) {
		freeNodes(noeud->next);
	}
	free(noeud);
}


void insert (struct node** ptrToNode, char* stringToInsert) {
	
	if (*ptrToNode == NULL) {
		struct node* nodeToInsert = create(stringToInsert);
		nodeToInsert->next = *ptrToNode;
		*ptrToNode = nodeToInsert;
		return;
	}

	if (strcmp( ( (*ptrToNode)->element), (stringToInsert) ) >= 0) {

		struct node* nodeToInsert = create(stringToInsert);
		
		// Le noeud inséré pointe sur le suivant...
		nodeToInsert->next = *ptrToNode;
		
		// ... et a pour prédecesseur le noeud précédent
		*ptrToNode = nodeToInsert;

		return;
	}
	else {
		
		insert(&((*ptrToNode)->next), stringToInsert);
	}
}


void destroyOneNode (struct node** ptrToNode, char* stringToDestroy) {
	
	if (strcmp( ( (*ptrToNode)->element), (stringToDestroy) ) == 0) {
		
		struct node* nodeToDestroy = *ptrToNode;

		*ptrToNode = (*ptrToNode)->next;

		free(nodeToDestroy);

		return;
	}
	else if ((*ptrToNode)->next != NULL) {
		
		destroyOneNode(&((*ptrToNode)->next), stringToDestroy);
	}
}


int main (int argc, char* argv[])
{

	struct node* n1 = NULL;
	//struct node* n2 = create("world !");
	
	//n1->next = n2;

	//printf("\nn1 contient %s\n\n", n1->element);
	//printf("n2 contient %s\n\n", n2->element);

	//display(n1);

	insert(&n1, " que ");
	insert(&n1, " la ");
	insert(&n1, " force ");
	insert(&n1, " soit ");
	insert(&n1, " avec ");
	insert(&n1, " toi ");
	insert(&n1, " luke ");

	display(n1);
	destroyOneNode(&n1, " luke ");
	display(n1);

	freeNodes(n1);
}






