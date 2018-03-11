/***********************************************************************
* Code contenant les fonctions qui doivent etre adaptees par           *
* l'etudiant pour son TP note                                          *
***********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void handle_connection (int connection_fd);

void *thread_gestion_connexion_entrante(void *arg);


/*************************************/
/* Definition des variables globales */
/*************************************/
int compteurReponse;

/********************************************/
/* Procedures appelees par le reste du code */
/********************************************/

/* Procedure chargee de toutes les initialisations specifiques au code de
   l'etudiant */
void init(){
  compteurReponse = 0; /* NB : On aurait aussi pu faire cette initialisation au moment */
                       /* de la declaration de compteurReponse, mais l'avantage de le  */
                       /* faire ici est que c'est plus pedagogique pour comprendre le  */
                       /* role de la procedure init.                                   */
}

/* Procedure chargee de la gestion d'une connexion entrante (liee a une requete 
   http */
void gestion_connexion_entrante(int connexion_fd){
  pthread_t thread;
  int rc;
  /* On cree le thread */
  rc = pthread_create(&thread, NULL, thread_gestion_connexion_entrante, (void*)connexion_fd);
  if (rc < 0){
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }
  /* On detache le thread de sorte que, quand il se terminera, les ressources */
  /* qu'il utilise pourront etre liberees.                                    */
  rc = pthread_detach(thread);
  if (rc < 0){
    perror("pthread_detach");
    exit(EXIT_FAILURE);
  }
}

/* Fonction chargee de faire les traitements lies a compteurReponse lorsque le serveur
   repond a une requete http. Cette focntion doit au minimum incrementer compteurReponse.
   Cette fonction renvoie la valeur de compteurReponse. */
int gestion_compteurReponse(){
  compteurReponse += 1;
  return compteurReponse;
}


/********************************************/
/* Procedures specifiques a l'etudiant      */
/********************************************/

/* Procedure chargee des traitements liees a une reponse a une requete http */
void *thread_gestion_connexion_entrante(void *arg){
  int connection_fd = (int) arg;
  handle_connection(connection_fd);
  pthread_exit(NULL);
}


