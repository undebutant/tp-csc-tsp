#include <stdlib.h>
#include <string.h>

extern char *message[];
extern int quelleEstLaLangue();

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

