// gcc -Wall -Werror -o executable main06.c -DFRANCAIS

#include <stdio.h>

int main() {
	puts(
#ifdef FRANCAIS
			"Bonjour le monde !"
#else
			"Hello world!"
#endif
			);
	return 0;
}
