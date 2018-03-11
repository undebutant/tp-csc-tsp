# Variable a adapter si ces programmes ne compilent pas (parce
# que le serveur utilise libevent et que libevent n'est pas 
# installe sur votre machine)
#
# ATTENTION : pour executer le serveur, ne pas oublier :
# export LD_LIBRARY_PATH=$HOME/Software/libevent-2.1.8-stable/.libs
LIBEVENTDIR=/home/guillaume/Developpement/TSP/CSC4508/ConceptionOS/Cours8_ClientServeur/libevent-2.1.8-stable

CFLAGS = -Wall -g
LDFLAGS = -g

EXE = clientTCP serveurTCP_1_tache_1_client serveurTCP_n_tache_n_client serveurTCP_pool_tache_n_client serveurTCP_1_tache_n_client

.PHONY: all clean

all: $(EXE)

clientTCP: clientTCP.o initCommunication.o
serveurTCP_1_tache_1_client: serveurTCP_1_tache_1_client.o initCommunication.o
	cc $^ -o $@ $(LDFLAGS)

serveurTCP_n_tache_n_client: serveurTCP_n_tache_n_client.o initCommunication.o
	cc $^ -o $@ $(LDFLAGS) -pthread

serveurTCP_pool_tache_n_client: serveurTCP_pool_tache_n_client.o initCommunication.o
	cc $^ -o $@ $(LDFLAGS) -pthread

serveurTCP_1_tache_n_client: serveurTCP_1_tache_n_client.o initCommunication.o
	cc $^ -o $@ $(LDFLAGS) -L${LIBEVENTDIR}/.libs -levent

%.o:%.c
	gcc -c $(CFLAGS) -I${LIBEVENTDIR}/include $<

clean:
	rm -f *~ *.bak *.o $(EXE)
