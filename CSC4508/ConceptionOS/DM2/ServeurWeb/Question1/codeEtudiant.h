/***********************************************************************
* Declarations des fonctions qui doivent etre adaptees par             *
* l'etudiant pour son TP note                                          *
***********************************************************************/


/* Procedure chargee de toutes les initialisations specifiques au code de
   l'etudiant */
void init();

/* Procedure chargee de la gestion d'une connexion entrante (liee a une requete 
   http */
void gestion_connexion_entrante(int connection_fd);

/* Fonction chargee de faire les traitements lies a compteurReponse lorsque le serveur
   repond a une requete http. Cette focntion doit au minimum incrementer compteurReponse.
   Cette fonction renvoie la valeur de compteurReponse. */
int gestion_compteurReponse();
