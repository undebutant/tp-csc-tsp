#include <stdio.h>
#include <errno.h>
#include <string.h>

int main (int argc, char *argv[]) {
	FILE * pf;

	pf = fopen ("unexist.txt", "rb");

	if (pf == NULL) {	
		fprintf(stderr, "Error opening file: %s\n", strerror(errno));
	}
	else {
		fclose (pf);
	}
	
	return 0;
}
