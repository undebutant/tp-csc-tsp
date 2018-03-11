#include <stdio.h>
#include <assert.h>




int main(int argc, char *argv[]) {

	long ligneVoulue;
	long ligneLue;
	long finLigneLue;
	char buf[2048];
	
	
	FILE *mEtMme = fopen("mEtMme.txt", "r");
	assert(mEtMme != NULL);
	FILE *indexFile = fopen("mEtMme.txt.idx", "r");
	assert(indexFile != NULL);


	printf("Entrez le numéro de la ligne voulue : ");
	assert (scanf("%li", &ligneVoulue) >= 0);

	fseek(indexFile, sizeof(long) * (ligneVoulue - 1), SEEK_SET);
	fread(&ligneLue, sizeof(long), 1, indexFile);
	fread(&finLigneLue, sizeof(long), 1, indexFile);


	fseek(mEtMme, ligneLue, SEEK_SET);
	fread(buf, finLigneLue - ligneLue, 1, mEtMme);

	printf("%s", buf);


	assert (fclose(mEtMme) == 0);
	assert (fclose(indexFile) == 0);
}
