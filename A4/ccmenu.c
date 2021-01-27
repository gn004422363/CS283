#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    int choice;
	int chpid;
	char arg1, arg2;
	char maker, model, cpu, desc, id, year;

	if(argc != 1) {
		fprintf(stderr, "Usage: ccmenu\n");
		exit(1);
	}
	
    // The do statement keep the menu continuously running
    do {
        // Print option menu
        printf("\nMenu");
        printf("\n1. ccadd");
		printf("\n2. ccitem");
		printf("\n3. cclist");
		printf("\n4. ccdel");
		printf("\n5. ccmatch");
		printf("\n6. ccyear");
		printf("\n7. ccedit");
		printf("\n8. Quit");
		printf("\nEnter your choice [1-8]: ");
        scanf("%d", &choice);

        // The switch statement read the user's input and perform the function
        switch(choice) 
        {
            case 1:
				printf("\nEnter the Maker: ");
				scanf("%s", &maker);
				argv[1] = &maker;

				printf("\nEnter the Model: ");
				scanf("%s", &model);
				argv[2] = &model;

				printf("\nEnter the Year: ");
				scanf("%s", &year);
				argv[3] = &year;

				printf("\nEnter the CPU: ");
				scanf("%s", &cpu);
				argv[4] = &cpu;

				printf("\nEnter the id: ");
				scanf("%s", &id);
				argv[5] = &id;

				printf("\nEnter the desc: ");
				scanf("%s", &desc);
				argv[6] = &desc;

				chpid = fork();
				if(chpid < 0) {
					perror("fork");
					exit(2);
				}
				if(chpid == 0) {
					execlp("./ccadd", "ccadd", argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], NULL);
					
				} else {
					system("wait");
					signal(SIGINT, SIG_IGN);
					wait((int *)0);
					sleep(1);
				}

				break;

            case 2:
				// Prompt id for search
				printf("\nEnter the item number: ");
				scanf("%s", &arg1);
				argv[1] = &arg1;

				chpid = fork();
				if(chpid < 0) {
					perror("fork");
					exit(2);
				}
				if(chpid == 0) {
					execlp("./ccitem", "ccitem", argv[1], NULL);
					
				} else {
					system("wait");
					signal(SIGINT, SIG_IGN);
					wait((int *)0);
					sleep(1);
				}

				break;

            
            case 3:
				chpid = fork();
				if(chpid < 0) {
					perror("fork");
					exit(2);
				}
				if(chpid == 0) {
					execl("./cclist", "cclist", NULL);
					
				} else {
					system("wait");
					signal(SIGINT, SIG_IGN);
					wait((int *)0);
					sleep(1);
				}

				break;

            case 4:
				// Prompt id for delete
				printf("\nEnter the item number: ");
				scanf("%s", &arg1);
				argv[1] = &arg1;

				chpid = fork();
				if(chpid < 0) {
					perror("fork");
					exit(2);
				}
				if(chpid == 0) {
					execlp("./ccdel", argv[1], NULL);
					
				} else {
					system("wait");
					signal(SIGINT, SIG_IGN);
					wait((int *)0);
					sleep(1);
				}

				break;

            
            case 5:
				// Prompt id for match
				printf("\nEnter string: ");
				scanf("%s", &arg1);
				argv[1] = &arg1;

				chpid = fork();
				if(chpid < 0) {
					perror("fork");
					exit(2);
				}
				if(chpid == 0) {
					execlp("ccmatch", "ccmatch", argv[1], NULL);
					
				} else {
					system("wait");
					signal(SIGINT, SIG_IGN);
					wait((int *)0);
					sleep(1);
				}

				break;

            
            case 6:
				// Prompt year1 for parameter
				printf("\nEnter the year1: ");
				scanf("%s", &arg1);
				argv[1] = &arg1;

				// Prompt year2 for parameter
				printf("\nEnter the year2: ");
				scanf("%s", &arg2);
				argv[2] = &arg2;
				
				chpid = fork();
				if(chpid < 0) {
					perror("fork");
					exit(2);
				}
				if(chpid == 0) {
					execlp("./ccyear", "ccyear", argv[1], argv[2], NULL);
					
				} else {
					system("wait");
					signal(SIGINT, SIG_IGN);
					wait((int *)0);
					sleep(1);
				}

				break;

			case 7:
				// Prompt id for edit
				printf("\nEnter the item number: ");
				scanf("%s", &arg1);
				argv[1] = &arg1;

				chpid = fork();
				if(chpid < 0) {
					perror("fork");
					exit(2);
				}
				if(chpid == 0) {
					execlp("./ccedit", "ccedit", argv[1], NULL);
					
				} else {
					system("wait");
					signal(SIGINT, SIG_IGN);
					wait((int *)0);
					sleep(1);
				}

				break;

            case 8:
                printf("exiting...\n");
				sleep(3);
				exit(0);
			
			default:
				printf("Usage:Invalid input, please enter between 1-8.\n");

        } // end switch

    } while (choice != 8);
	
}
