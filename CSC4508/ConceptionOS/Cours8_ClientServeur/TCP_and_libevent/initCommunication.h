/***********************/
/* initCommunication.h */
/***********************/

#ifndef _INITCOMMUNICATION_H
#define _INITCOMMUNICATION_H

// Fonction utilisee cote client
// Elle se connecte au point d'acces TCP correspondant au couple
// hote HOST / port PORT.
// Renvoie le file descriptor de ce point d'acces (ou bien quitte
// le programme s'il y a eu une erreur).
int connexionPointDAcces();

// Fonction utilisee cote serveur
// Elle cree le point d'acces TCP correspondant au port PORT.
// Renvoie le file descriptor de ce point d'acces (ou bien quitte
// le programme s'il y a eu une erreur).
int creationPointDAcces();

#endif /* _INITCOMMUNICATION_H */
