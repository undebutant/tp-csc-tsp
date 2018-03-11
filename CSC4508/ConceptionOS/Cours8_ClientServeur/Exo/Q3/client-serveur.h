/*************************************************/
/* Declarations communes au client et au serveur */
/*************************************************/

#define NOM_POINT_ACCES_SERV "/client-serveur"

#define TAILLE_MAX_POINT_ACCESS 32
#define TAILLE_MAX_CONTENU 256

/* Structure des messages utilises dans le sens client-->serveur */
/* NB : les 2 chaines de caracteres se terminent par '\0'        */
typedef struct{
  char pointAccesClient[TAILLE_MAX_POINT_ACCESS];
  char contenu[TAILLE_MAX_CONTENU];
} requete_t;

/* Structure des messages utilises dans le sens serveur-->client */
/* NB : les 2 chaines de caracteres se terminent par '\0'        */
typedef struct{
  char pointAccesPrive[TAILLE_MAX_POINT_ACCESS];
  char contenu[TAILLE_MAX_CONTENU];
} reponse_t;
