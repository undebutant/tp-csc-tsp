#include <stdio.h>

extern char *message[];
extern int quelleEstLaLangue();

int main() {
	puts(message[quelleEstLaLangue()]);
	return 0;
}
