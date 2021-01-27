#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include "cc.h"

int
main(int argc, char *argv[])
{
	CComp new;
	FILE *fp;

	if(argc != 7) {
		fprintf(stderr, "Usage: ccadd id maker model year cpu desc\n");
		exit(1);
	}

	if (!strcmp(argv[1], "-a")) {
		fstat(&fp, new.id);




	} else {
		new.id = atoi(argv[1]);
		strncpy(new.maker, argv[2], Nmaker-1);
		new.maker[Nmaker-1] = '\0';
		strncpy(new.model, argv[3], Nmodel-1);
		new.model[Nmodel-1] = '\0';
		new.year = atoi(argv[4]);
		strncpy(new.cpu, argv[5], Ncpu-1);
		new.cpu[Ncpu-1] = '\0';
		strncpy(new.desc, argv[6], Ndesc-1);
		new.desc[Ndesc-1] = '\0';
	}

	fp = fopen("ccomp.db", "r+");
	if(fp == NULL) {
		perror("fopen");
		exit(2);
	}

	// File lock by flock()
	flock(fileno(fp), LOCK_SH);

	// Seek the data
	fseek(fp, new.id * sizeof(CComp), SEEK_SET);

	// Write data to the db
	fwrite(&new, sizeof(CComp), 1, fp);

	// Unlock file
	flock(fileno(fp), LOCK_UN);

	// Close file
	fclose(fp);
	exit(0);
}