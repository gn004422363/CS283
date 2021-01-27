#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include "cc.h"

int
main(int argc, char *argv[])
{
	CComp target;
	FILE *fp;
	int year1, year2;
    int index;

	if(argc != 3) {
		fprintf(stderr, "Usage: ccyear year1 year2\n");
		exit(1);
	} 

	// Open file with r+ premission
	fp = fopen("ccomp.db", "r+");

	// Assign argument value to index
	year1 = atoi(argv[1]);
    year2 = atoi(argv[2]);

    if (year2 < year1) {
        fprintf(stderr, "Usage: invalid year\n");
		exit(1);
    } 

	// File lock by flock()
	flock(fileno(fp), LOCK_SH);

	// Pointer to the target index from ccomp
	fseek(fp, sizeof(CComp), SEEK_SET);
    
    // Index starts at 1
    index = 1;

    // Loop entire database and print the year between year1 and year2
    while(fread(&target, sizeof(CComp), 1, fp) > 0) {
		if(year1 <= target.year && year2 >= target.year) {
			printf("\n");
			printf("Maker: %s\n", target.maker);
			printf("Model: %s\n", target.model);
			printf("Year: %d\n", target.year);
			printf("CPU: %s\n", target.cpu);
			printf("Id: %d\n", target.id);
			printf("Desc: %s\n", target.desc);
			printf("----------------\n");
		}
		index++;
	}

	// Read data from the pointer
	fread(&target, sizeof(CComp), 1, fp);

	// Unlock file
	flock(fileno(fp), LOCK_UN);

	// Close file
	fclose(fp);

	exit(0);
}