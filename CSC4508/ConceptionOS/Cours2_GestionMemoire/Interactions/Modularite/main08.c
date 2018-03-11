#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *message[] = {
	"Hello world!",
	"Bonjour le monde !"
};

int quelleEstLaLangue() {
	if (! strcmp(getenv("LANG"), "fr_FR.UTF-8")) {
		return 1;
	}
	else {
		return 0;
	}
}

int main() {
	puts(message[quelleEstLaLangue()]);
	return 0;
}
