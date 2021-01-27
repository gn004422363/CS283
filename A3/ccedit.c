#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <ctype.h>
#include "cc.h"

int
main(int argc, char *argv[])
{
	CComp target;
	FILE *fp;
	int index;
   char response, string_input;
   int int_input, change_input;

	if(argc != 2) {
		fprintf(stderr, "Usage: ccedit id\n");
		exit(1);
	} 

	// Open file with r+ premission
	fp = fopen("ccomp.db", "r+");

	// Assign argument value to index
	index = atoi(argv[1]);

	// File lock by flock()
	flock(fileno(fp), LOCK_SH);

	// Set the file position indiator to the desired position
	fseek(fp, index * sizeof(CComp), SEEK_SET);

	// read the data
	fread(&target, sizeof(CComp), 1, fp);

	if(index != target.id) {
		fprintf(stderr, "No such item\n");
		exit(2);
	} 

   fprintf(stderr, "\n\nOriginal:\n");
	printf("Maker: %s\n", target.maker);
	printf("Model: %s\n", target.model);
	printf("Year: %d\n", target.year);
	printf("CPU: %s\n", target.cpu);
	printf("Id: %d\n", target.id);
	printf("Desc: %s\n", target.desc);

   // Ask if user want to edit
   fprintf(stderr, "Do you want to modify? [y/n]: ");
   scanf("%c", &response);
	response = tolower(response);

   // Change value menu
   if (response == 'y') {
        fprintf(stderr, "\n1. Marker\n2. Model\n3. Year\n4. CPU\n5. Desc\nWhat do you want to edit? [1-5]: ");
        scanf("%d", &int_input);

        if (int_input == 1) {
            fprintf(stderr, "Enter your change for marker: ");
            scanf("%s", &string_input);
            strcpy(target.maker, &string_input); 

        } else if(int_input == 2) {
            fprintf(stderr, "Enter your change for model: ");
            scanf("%s", &string_input);
            strcpy(target.model, &string_input); 

        } else if(int_input == 3) {
            fprintf(stderr, "Enter your change for year: ");
            scanf("%d", &change_input);
            target.year = change_input;

        } else if(int_input == 4) {
            fprintf(stderr, "Enter your change for cpu: ");
            scanf("%s", &string_input);
            strcpy(target.cpu, &string_input); 

        } else if(int_input == 5) {
            fprintf(stderr, "Enter your change for description: ");
            scanf("%s", &string_input);
            strcpy(target.desc, &string_input);
		  }

    } else if (response == 'n') {
        fprintf(stderr, "\nexisting...\n");
        exit(0);
    }

	// Write the data
	fseek(fp, index * sizeof(CComp), SEEK_SET);
	fwrite(&target, sizeof(CComp), 1, fp);

	//Set the file position indiator to the desired position
	fseek(fp, index * sizeof(CComp), SEEK_SET);
	// read the data
	fread(&target, sizeof(CComp), 1, fp);

	// Read back
	fprintf(stderr, "\n\nUpdated item:\n");
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
