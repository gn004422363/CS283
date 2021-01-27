#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

	// Set the file position indiator to the desired position
	fseek(fp, index*sizeof(CComp), SEEK_SET);

	// read the data
	fread(&target, sizeof(CComp), 1, fp);

	if(index != target.id) {
		fprintf(stderr, "No such item\n");
		exit(2);
	} 

    printf("\n\nOriginal:");
	printf("Maker: %s\n", target.maker);
	printf("Model: %s\n", target.model);
	printf("Year: %d\n", target.year);
	printf("CPU: %s\n", target.cpu);
	printf("Id: %d\n", target.id);
	printf("Desc: %s\n", target.desc);

	// Change value
	strcpy(target.maker, "deleted"); 
	strcpy(target.model, "deleted"); 
	target.year = 0;
	strcpy(target.cpu, "deleted"); 
	target.id = 0;
	strcpy(target.desc, "deleted"); 

	// Write the data
	fseek(fp, index*sizeof(CComp), SEEK_SET);
	fwrite(&target, sizeof(CComp), 1, fp);

	//Set the file position indiator to the desired position
	fseek(fp, index*sizeof(CComp), SEEK_SET);
	// read the data
	fread(&target, sizeof(CComp), 1, fp);

	// Read back
	printf("\n\nDeleted item:\n");
	printf("Maker: %s\n", target.maker);
	printf("Model: %s\n", target.model);
	printf("Year: %d\n", target.year);
	printf("CPU: %s\n", target.cpu);
	printf("Id: %d\n", target.id);
	printf("Desc: %s\n", target.desc);

	// Unlock file
	flock(fileno(fp), LOCK_UN);

	// Close the file
	fclose(fp);

	exit(0);
}