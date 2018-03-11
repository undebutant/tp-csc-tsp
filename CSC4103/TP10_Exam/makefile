
CFLAGS=-Wall -Werror


all : server client


server: server.c server_code_etudiant.c server_code_etudiant.h
		gcc -o server server.c server_code_etudiant.c server_code_etudiant.h -g $(CFLAGS)


client: client.c client_code_etudiant.c client_code_etudiant.h
		gcc -o client client.c client_code_etudiant.c client_code_etudiant.h -g $(CFLAGS)


clean:
	rm -rf *.o server client
