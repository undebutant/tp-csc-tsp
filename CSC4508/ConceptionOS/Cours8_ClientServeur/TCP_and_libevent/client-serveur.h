/********************/
/* client-serveur.h */
/********************/

#ifndef _CLIENT_SERVEUR_H
#define _CLIENT_SERVEUR_H

/* Declarations communes au client et au serveur */

/** Hote auquel se connecte le client
 */
#define HOST "localhost"

/** Port sur lequel le serveur est tout le temps a l'ecoute.
 */
#define PORT "2048"

/** Taille maximum d'un identifiant client */
#define TAILLE_ID_CLIENT 64

/** Taille maximum d'une payload dans un message */
#define TAILLE_PAYLOAD 256

/**
 * \enum typMessage_t
 * \brief Type de message circulant entre le client et le serveur.
 *
 * typMessage_t définit les types de message circulant dans 
 * les sens client-->serveur et serveur-->client
 */
typedef enum
{
  REQUETE,        /*!< (sens client-->serveur) Requete. */
  REQUETE_OK,     /*!< (sens serveur-->client) Reponse positive a la requete. */
  REQUETE_KO      /*!< (sens serveur-->client) reponse negative a la requete. */
}
typMessage_t;

/**
 * \struct message_t
 * \brief Message circulant entre le client et le serveur.
 *
 * message_t définit les structures de message circulant dans les 
 * sens client-->serveur et serveur-->client
 */
typedef struct
{
  typMessage_t typ; /*!< Type de message */
  union {
    char idClient[TAILLE_ID_CLIENT]; /*!< Identifiant client. */
    char payload[TAILLE_PAYLOAD]; /*!< Payload associee a la reponse. */
  } infoSup; /*!< Info supplementaire associee a ce message. */
}
  message_t;

#define CALCUL_PAYLOAD(s, id, nb) sprintf(s, "%2d-ieme reponse au client '%6s'", nb, id)

#endif /* _CLIENT_SERVEUR_H */
