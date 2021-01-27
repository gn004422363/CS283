#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include "cc.h"

int
main(int argc, char *argv[])
{
	CComp target;
	FILE *fp;
	int index;

	if(argc != 2) {
		fprintf(stderr, "Usage: ccitem id\n");
		exit(1);
	} 

	// Open file with r+ premission
	fp = fopen("ccomp.db", "r+");

	// Assign argument value to index
	index = atoi(argv[1]);

	// File lock by flock()
	flock(fileno(fp), LOCK_SH);

	// Pointer to the target index from ccomp
	fseek(fp, index * sizeof(CComp), SEEK_SET);

	// Read data from the pointer
	fread(&target, sizeof(CComp), 1, fp);

	// Unlock file
	flock(fileno(fp), LOCK_UN);

	// Close file
	fclose(fp);

	if(index != target.id) {
		fprintf(stderr, "No such item\n");
		exit(2);
	}
	printf("Maker: %s\n", target.maker);
	printf("Model: %s\n", target.model);
	printf("Year: %d\n", target.year);
	printf("CPU: %s\n", target.cpu);
	printf("Id: %d\n", target.id);
	printf("Desc: %s\n", target.desc);
	exit(0);
}