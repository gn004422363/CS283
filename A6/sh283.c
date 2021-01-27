/* C Libraries */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX 100
#define LEN 100

/* Structure for single command*/
struct cmd_list {
    int argc; // command
    char *argv[MAX]; // Arguments
};

struct cmd_list *cmdv[MAX]; // Shell command
int num; // counter
int flagdo; // To determine whether is backgrounding commands 

/* Display welcome screen at the beginning when run the program*/
void welcomeScreen()  {
    printf("\n============================================\n");
    printf("\n\t          CS 283\n");
    printf("\t Assignment 6 - C Shell\n");
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

/* Execute command and check command whether it's valid*/
void execute(char *argv[])
{
    int invalid_cmd;
    invalid_cmd = execvp(argv[0], argv);

    // Check for validtion of command
    //if value is -1, then will print out error message
    if (invalid_cmd == -1) 
    {
        printf("Invalid command!\n");
    }
    exit(1);
}

/* Split the command */
void split(char *line)
{
    // Assigning memory to struct cmd
    struct cmd_list*cmd = (struct cmd_list *)malloc(sizeof(struct cmd_list));
 
    char *save;
    char *arg = strtok_r(line, " \t", &save); // Split space
    cmdv[num++] = cmd;

    // Assigning value to argc variable
    // of cmd using arrow operator 
    cmd->argc = 0; 
    
    //
    while(arg)
    {
        cmd -> argv[cmd -> argc] = arg;
        arg = strtok_r(NULL, " \t", &save);
        cmd -> argc++;
    }
    cmd->argv[cmd->argc] = NULL;
}

// Split pipeline
void split_pipe(char *line)
{
    char *save;
    char * cmd = strtok_r(line, "|", &save);

    // Splitting strings into a sequence of tokens and check whether pipe is exist
    while(cmd) 
    {
        split(cmd);
        cmd = strtok_r(NULL, "|", &save);
    }
}

// This function execute system build in command
int build_in(char *line)
{
    int i = 0;
    char *save, *tmp[MAX];
    char t[LEN], p[LEN];
    char *arg = strtok_r(line, " \t", &save); // split space
    strcpy(t, line); // Copy string from line to t

    // Record each argument
    while(arg) 
    {
        tmp[i] = arg;
        i++;
        arg = strtok_r(NULL, " \t", &save);
    }
    tmp[i] = NULL;
    if (strcmp(tmp[i-1], "&") == 0) // To check if is background command
    {
        flagdo = 1;
        i--;
    }
    if (strcmp(tmp[0], "cd") == 0) // cd command
    {
        char buf[LEN]; // buffer
        if (chdir(tmp[1]) >= 0)
        {
            getcwd(buf, sizeof(buf));
            printf("Moved to: %s\n", buf);
        } 
        else
        {
            printf("Invalid Path!\n");
        }
        return 1;
    }
    else if(strcmp(tmp[0], "pwd") == 0) // pwd command
    {
        char buf[LEN];
        getcwd(buf, sizeof(buf)); // To get the current location
        printf("Current dir is: %s\n", buf);
        return 1;
    } 
    else if(strcmp(tmp[0], "exit") == 0) // exit command
    {
        printf("Terminated, goodbye!\n");
        exit(0);
        return 1;
    }
    else if (strcmp(tmp[0], "help") == 0) // help command
    { 
        openHelp();
        return 1;
    }
    else if (strcmp(tmp[0], "pid") == 0) // pid command
    {
        printf("pid: %d\n", getpid());
        return 1;
    }
    else return 0;
}


// The function execute the command of pipeline
void execute_pipe(int index)
{
    int fd[2];

    if (index == num - 1){
        execute(cmdv[index] -> argv);
    }

    // Create child process. Read, if is 0. Write, if is 1.
    pipe(fd);

    if(fork() < 0) 
    {
        perror("fork");
        exit(EXIT_FAILURE);

    } else if(fork() == 0) 
    {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execute(cmdv[index] -> argv);

    } else {
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        execute_pipe(index+1);
    }
}

// This function take input file and redirection
void input(char *q)
{
    char tmp[30];
    int fd;
    if (q[0] == '<')
    {
        strcpy(tmp, q+1);
        fd = open(tmp, O_RDONLY);

        // Fill argv[]
        // Set argc
        cmdv[0]->argv[cmdv[0]->argc - 1] = NULL;
        cmdv[0]->argc--;

        // If the value is -1, then will print out failed message
        if (fd == -1)
        {
            printf("Failed to open file\n");
            return;
        }
        dup2(fd, 0);
        close(fd);
    }
}

void output(char *q)
{
    char tmp[30];
    int fd;

    // Output file redireciton for truncating
    if (q[0] == '>')
    {
        strcpy(tmp, q+1);

        // set argc
        cmdv[num-1] -> argv[cmdv[num-1] -> argc-1] = NULL;
        cmdv[num-1] -> argc --;
        fd = open(tmp, O_RDWR|O_CREAT, 0666); // open file with read and write premission
        if (fd == -1)
        {
            printf("Failed to open file\n");
            return;
        }
        
        dup2(fd, 1);
        close(fd);
    }
    // Output file redireciton for appending
    else if (q[0] == '>' && q[0] == '>')
    {
        strcpy(tmp, q+1);

        // set argc
        cmdv[num-1] -> argv[cmdv[num-1] -> argc-1] = NULL;
        cmdv[num-1] -> argc --;
        fd = open(tmp, O_RDWR|O_CREAT, 0666); // open file with read and write premission
        if (fd == -1)
        {
            printf("Failed to open file\n");
            return;
        }
        
        dup2(fd, 1);
        close(fd);
    }
}

int main(int argc, char ** argv)
{
    char buf[LEN], p[LEN];
    int i, pid;

    // Display welcome screen
    welcomeScreen();
    while (1)
    {
        int inner;
        printf(">>> ");
        fgets(buf, LEN, stdin); // Read shell command
        if (buf[0] == '\n') continue;
        buf[strlen(buf)-1] = '\0';

        // Copy content from p to buffer
        strcpy(p, buf);

        // Build command
        inner = build_in(buf);
        if (inner == 0)
        {
            pid = fork();
            if (pid == 0)
            {
                // Split and handle pipeline
                split_pipe(p);

                // Delete & sign if detect there's background notation
                if (strcmp(cmdv[num-1]->argv[cmdv[num-1]->argc-1], "&") == 0)
                {
                    // Set argc
                    cmdv[num-1]->argc--;
                    cmdv[num-1]->argv[cmdv[num-1]->argc] = NULL;
                }

                // Defaulting the last value as both input and output value
                // The first pipe in input value, the last pipe in output value
                if (cmdv[0]->argv[cmdv[0]->argc-1]!=NULL)
                {
                    char q[LEN];
                    strcpy(q,cmdv[0]->argv[cmdv[0]->argc-1]);
                    input(q); 
                }

                // Defaulting the last value as both input and output value
                // The first pipe in input value, the last pipe in output value
                if (cmdv[num-1]->argv[cmdv[num-1]->argc-1]!=NULL)
                {
                    char q[LEN];
                    strcpy(q,cmdv[num-1]->argv[cmdv[num-1]->argc-1]);
                    output(q); 
                }
                execute_pipe(0); // execute pipeline
                exit(0);
            }
            
            // If the value of backgrounding command is 0 then waiting for input value
            if (flagdo == 0) 
            {
                waitpid(pid, NULL, 0);
            }
        }
    }
    return 0;
}