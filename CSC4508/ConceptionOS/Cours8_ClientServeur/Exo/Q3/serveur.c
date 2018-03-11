#include <mqueue.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <error.h>
#include <assert.h>
#include <unistd.h>

#include "client-serveur.h"



int descripteurTube[26][2];

union myInt{
	int value;
	void* ptr;
};

void * traiterRequete(void*arg){
  union myInt inta;
  inta.ptr = arg; 
  
  
  int firstline = 0;
  int lastline = 0;
  int firsttime = 1;
  int counter = 0;
  FILE *f;
  char ligne[TAILLE_MAX_CONTENU];
  
  f = fopen("mEtMme.txt", "r");
	  if (f == NULL) {
	    perror("open(mEtMme.txt)");
	    exit(EXIT_FAILURE);
  }
  while (feof(f) == 0) {
	  if (fgets(ligne, sizeof(ligne), f) != NULL) {
	      if (ligne[0] == (inta.value + 'A')){
		 if(firsttime == 1) {
		 	firstline = counter;
                        firsttime = 0;
		 }
	      } else {
	    	 if(firsttime==0) {
			lastline=counter-1;
     			firsttime = 2;
		 }
	      }
         
              counter ++;
	  }
  }
  if(lastline<firstline) {
	lastline=counter -1;
  }
  
  if (fclose(f) < 0) {
	    perror("fclose");
	    exit(EXIT_FAILURE);
  }

  printf("value of my thread :%d:\nfirstline :%d:\nlastline :%d:\n",inta.value,firstline,lastline);
  while(1) {
	 requete_t * pRequete = NULL;
	 ssize_t size;
   	 size = read(descripteurTube[inta.value][0],(void*)&pRequete,sizeof(requete_t *));
    	 printf("size reçu :%zu:\n",size);
    	 if(size==sizeof(pRequete)) {
      	   	printf("Je gère la connection :%d:\n",inta.value);
       	  	
  		
	  
	  reponse_t reponse;
	  mqd_t mqReponses;
	  FILE *f;
	  char ligne[TAILLE_MAX_CONTENU];
	  int len;

	  /* Calcul reponse.pointAccesPrive */
	  strcpy(reponse.pointAccesPrive, "");
		      /* Le point d'acces prive n'est pas utilise dans cette question */

	  /* Calcul reponse.contenu */
	  f = fopen("mEtMme.txt", "r");
	  if (f == NULL) {
	    perror("open(mEtMme.txt)");
	    exit(EXIT_FAILURE);
	  }
	  while (feof(f) == 0) {
	    if (fgets(ligne, sizeof(ligne), f) != NULL) {
	      if (strncmp(ligne, pRequete->contenu, strlen(pRequete->contenu)) == 0){
		break;
	      }
	    }
	  }
	  if (feof(f) == 0) {
	    strcpy(reponse.contenu, ligne);
	    reponse.contenu[strlen(ligne)-1] = '\0'; /* Supprime le '\n' final */
	  }
	  else {
	    strcpy(reponse.contenu,"Desole, je ne le connais pas");
	  }
	  if (fclose(f) < 0) {
	    perror("fclose");
	    exit(EXIT_FAILURE);
	  }

	  /* Affichage */
	  printf("Serveur a recu \"%s\" et repond \"%s\"\n",
		 pRequete->contenu,
		 reponse.contenu );

	  /* Connexion au point d'acces client et reponse */
	  mqReponses = mq_open(pRequete->pointAccesClient, O_WRONLY);
	  if (mqReponses < 0) {
	    perror("Server: mq_open");
	    exit(EXIT_FAILURE);
	  }
	  len = mq_send(mqReponses, (char*)&reponse, sizeof(reponse), 1);
	  if (len < 0) {
	    perror("Server: mq_send");
	    exit(EXIT_FAILURE);
	  }

	  /* Dans cette question, le client ne renvoie pas de requete ulterieure */
	  /* necessitant une reponse ==> On peut fermer cette message queue      */
	  if(mq_close(mqReponses) < 0){
	    perror("Serveur: mq_close");
	    exit(EXIT_FAILURE);
	  }
   	  free(pRequete);
   	 } 

          
  }
  return NULL;
}

int main() {
  mqd_t mqRequetes;
  struct mq_attr attr;
  int len;
  for(int i = 0;i<26;i++) {
	int tube = pipe(descripteurTube[i]);
  
  	assert(tube==0);
  }
  
  /* Creation de la message queue pour recevoir les requetes */
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;		/* size of the message queue */
  attr.mq_curmsgs = 0;
  attr.mq_msgsize = sizeof(requete_t); /* size of one message */
  mqRequetes = mq_open(NOM_POINT_ACCES_SERV, O_CREAT|O_RDONLY, S_IRUSR|S_IWUSR, &attr);
  if (mqRequetes < 0) {
    perror("Server: mq_open");
    exit(EXIT_FAILURE);
  }

  
  

  for(int i =0;i<26;i++) {
     union myInt inti;
     inti.value=i;
     pthread_t thread;
     int rc = pthread_create(&thread, NULL, traiterRequete, inti.ptr);
     if (rc < 0)
       error_at_line(EXIT_FAILURE, rc, __FILE__, __LINE__, "pthread_create");
     rc = pthread_detach(thread);
     if (rc < 0)
       error_at_line(EXIT_FAILURE, rc, __FILE__, __LINE__, "pthread_detach");

  }
  
  

  /* Attente de requetes */
  do{
    requete_t * requete = malloc(sizeof(requete_t));
    len = mq_receive(mqRequetes, (char*)requete, sizeof(requete_t), NULL);
    if(len < sizeof(requete_t)) {
      if (len < 0) {
	perror("Server: mq_receive");
      } else {
	fprintf(stderr, "Bizarre : on n'a recu que %d octets alors qu'on en attendait %zu !\n", len, sizeof(requete));
      }
      exit(EXIT_FAILURE);
    }
    /* On traite la requete */
    
    char firstletter = (requete->contenu)[0];
    printf("firstletter :%c:\n",firstletter);
    firstletter=firstletter-'A';
    ssize_t size;
    size = write(descripteurTube[(int)firstletter][1], (void*)&requete, sizeof(requete));
    printf("size envoye :%zu:\n",size);
    assert(size==sizeof(requete));
    
    //traiterRequete(&requete);
  }while(1);
}


   
