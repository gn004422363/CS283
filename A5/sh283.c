/* C Libraries */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>

char parsecmd(char *);
char *argv[8];
int argc=0;

/* Display welcome screen at the beginning when run the program*/
void welcomeScreen()  {
    printf("\n============================================\n");
    printf("\n\t          CS 283\n");
    printf("\t Assignment 5 - C Shell\n");
    printf("\n============================================\n");

    sleep(1);
}

/*  Display available commands */
void openHelp() 
{
    puts("\n*** AVAILABLE COMMANDS ***"
        "\nCopyright @ MAN TIK LI"
        "\nSupported Commands:"
        "\n>cd"
        "\n>ls"
        "\n>pwd"
        "\n>pid"
        "\n>rm"
        "\n>mkdir"
        "\n>mv"
        "\n>cp"
        "\n>exit"); 
}

/* Function to execute builtin commands */
char parse_cmd(char *s)
{
    if(!strcasecmp(s, "exit")) return 'T'; /* return string true, if user input exit command*/
    else if(!strncasecmp(s, "cd", 2)) return 1;
    else if(!strncasecmp(s, "ls", 2)) return 2;
    else if(!strncasecmp(s, "pid", 3)) return 3;
    else if(!strncasecmp(s, "rm", 2)) return 4;
    else if(!strncasecmp(s, "mkdir", 5)) return 5;
    else if(!strncasecmp(s, "mv", 2)) return 6;
    else if(!strncasecmp(s, "cp", 2)) return 7;
    else if(!strncasecmp(s, "pwd", 3)) return 8;
    else if(!strncasecmp(s, "help", 2)) return 9;
    else return 0;
}

/* Function for the system command to executed */
void do_execute(void)
{
    pid_t pid = fork();
    switch(pid)
    {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
            break;

        /* Child process */
        case 0:
            execvp(argv[0], argv);
            perror("execvp");
            exit(EXIT_FAILURE);

        /* Parent process*/
        dafault:
        {
            int st;
            while(wait(&st)!=pid);
        }
    }
}

int main(int argc, char ** argv)
{
    char cmd[1024];
    char tmp[101];
    char input_cmd = 'F';
    char * p;


    welcomeScreen();

    while(1) {
        char **input = memset(cmd, 0, 1024);
        printf(">>> ");/* Display prompt */

        
        fgets(cmd, 1000, stdin); /* User's input support 10000 characters */
        cmd[strlen(cmd) - 1] = 0; /* String length */

        /* Pasrse input argument */
        input_cmd = parse_cmd(cmd);

        do_execute();

        /* Exist shell, if user input 'exit' */
        if(input_cmd == 'T') { 
            printf("Terminated\n");
            printf("Goodbye!\n");
            exit(0);
        }
        
        switch(input_cmd)
        {
            case 1:/* cd command */
                if(chdir(cmd + 3) != 0) {
                    printf("chdir(%s) error!%s\n", cmd + 3, strerror(errno));
                }
            
                // Display directory
                printf("Moved to '%s'\n", getcwd(tmp, 100));
                break;

            case 2:/* ls command */
                system(cmd);
                break;

            case 3:/* pid command */
                printf("pid: %d\n", getpid());
                break;

            case 4:/* rm command */
                remove(cmd + 3);
                break;

            case 5:/* mkdir command */
                mkdir(cmd + 6, 0755);
                break;

            case 6:/* mv command */
                p = strchr(cmd + 3, ' ');
                *p = 0;
                rename(cmd + 3, p + 1);
                break;

            case 7:/* cp command */

            case 8:/* pwd command */

            case 9: /* help command */
                openHelp();
                break;

            default: /* Error message for invalid command */
                printf("INVALID COMMAND! (Type 'help' to find the available commands)\n");
                break;
        }
    }
}