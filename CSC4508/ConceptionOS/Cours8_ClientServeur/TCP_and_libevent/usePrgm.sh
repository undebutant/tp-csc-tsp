#!/bin/bash

# Si on fait
for((i=1;i<=600;i+=1)); do (./clientTCP $i &);done
# Pas de probleme de co, assez peu de clients


# Si on passe a 1200 clients, faisant une requete a la seconde...
#for((i=1;i<=1200;i+=1)); do (./clientTCP $i &);done
# On depasse le quota de ressource simultane, voir ulimit -a
# -n: file descriptors                1024
#
# Solution : faire un ulimit -n 2048
# Mais ne fait que repousser le probleme !
#
# Meilleures solutions :
# - Semaphore a 1000 par exemple, et nouveau pthread a chaque fois
# - creer un millier de thread (pool de thread) auquel on assigne le client.
#   Pour ce faire, il faut leur donner une instruction bloquante (ici le accept de gestionPointDAcces),
#   qui se degagera lorsqu'on en aura besoin (en cas de connexion)
