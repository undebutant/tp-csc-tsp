#include <stdio.h>
#include <assert.h>
#include <stdlib.h>




int main (void) {
	FILE* fs = fopen("mEtMme.txt","r");
	assert(fs!=NULL);
	
	FILE* fd = fopen("mEtMme.txt.idx","w");
	assert(fd!=NULL);
	
		

	long i = ftell(fs);
	fwrite(&i,sizeof(i),1,fd);
	
	char *a = malloc(sizeof(char)*2048);
	while((fgets(a,100,fs))!=NULL) {
		i = ftell(fs);
		fwrite(&i,sizeof(i),1,fd);
	}

	
	free(a);
	assert(fclose(fd)==0);
	assert(fclose(fs)==0);
}
