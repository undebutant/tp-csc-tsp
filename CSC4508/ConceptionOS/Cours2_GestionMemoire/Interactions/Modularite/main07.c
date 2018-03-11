// gcc -Wall -Werror -o executable main07.c -DMESSAGE='"Bonjour le monde !"'
// Remarque : les variables de preprocesseur sont conventionnelement TOUJOURS en majuscules

#include <stdio.h>

int main() {
	puts(MESSAGE);
	return 0;
}
