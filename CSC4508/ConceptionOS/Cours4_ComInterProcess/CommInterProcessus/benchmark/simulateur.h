/**
   @file simulateur.h

   Declaration des fonctions et procedures fournies par le module simulateur
*/

/**
   Taille max des requetes gerees par ce serveur
*/
#define TAILLE_MAX_REQUETE 4096

/**
   Initialisation du simulateur
   @param nbReq (donnee) Nombre de requetes que devra traiter le simulateur
*/
void initSimulateur(int nbReq);

/**
   Simulation de la reception d'une requete envoyee par un client au serveur.
   @param req (donnee-resultat) Zone ou doit etre stockee la requete que le serveur est suppose avoir recu du client.
*/
void simulerReceptionRequeteDeClient(char *req);

/**
   Simulation du traitement de la requete @a req et de son envoi vers le client
   @param req (donnee) Zone ou est stockee la requete a traiter
*/
void simulerTraitementEtEnvoiReponseAuClient(char *req, int numThread);


