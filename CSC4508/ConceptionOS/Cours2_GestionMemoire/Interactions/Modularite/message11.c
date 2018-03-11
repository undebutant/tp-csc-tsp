#include <stdlib.h>
#include <string.h>
#include "message11.h"

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

