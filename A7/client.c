#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

#define STRMAX 100
int socketfd;

// Recieve server socket by thread
void *recvsocket(void *arg)
{
	char buf[255]= {};
    while(1)
    {
        int ret = recv(socketfd,buf,sizeof(buf),0 );
        if(0 > ret)
        {
            perror("recv");
            exit(1);
        }
        printf("%s\n",buf);
    }
    return NULL;
}
 
// Send thread to server socket
void *sendsocket(void *arg)
{
	int st = *(int *)arg;
	char s[1024];
	while(1)
	{
		memset(s, 0, sizeof(s));
		read(STDIN_FILENO, s, sizeof(s)); // Read user input
		send(st, s, strlen(s), 0);
	}
	return NULL;
}
 
int main(int argc, char *argv[])
{
    int port, n;
    char recvbuf[255] = {};
    char sendbuf[255] = {};
    unsigned int ret ;
    struct hostent *he; // Server host name info
	struct sockaddr_in server; // IP strucutre

    // Check arguments
	if(argc != 2) 
    {
        fprintf(stderr, "Usage: client 'STUDENT NAME'\n");
        exit(1);
    }

    ret = socketfd = socket(AF_INET, SOCK_STREAM, 0);

    // Define the Internet socket,TCP
    if (ret < 0) 
    {
        perror("socket() error\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    // Get server host from server name.
    he = gethostbyname("tux3");

    // Check host 
    if (he == NULL)
    {  
        printf("gethostbyname() error\n");
        printf("%s\n", strerror(errno));
        exit(1); 
    } 

    port = 2020;

    // Set socket structures with null values
    bzero(&server,sizeof(server));

    /* Connect to server */
	server.sin_family = AF_INET; // Address family is internet
	server.sin_port = htons(2020); // set port to 2020
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the created socket to the specified port
    ret = bind(socketfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
    if (ret < 0)
    {
        fprintf(stderr, "%s: error: cannot bind socket to port %d\n", argv[0], port);
        shutdown(socketfd, SHUT_RDWR);
        exit(1);
    } 
    else
    {
        printf("Port listening\n");
    }

    // Connect to server
    server.sin_addr = *((struct in_addr *)he->h_addr);

	// Check if it's connect to target ip address. Return error if the status is -1
    ret = connect(socketfd, (struct sockaddr *) &server, sizeof(server));
	if (ret < 0)
	{
		printf("connect failed\n %s\n", strerror(errno));
        shutdown(socketfd, SHUT_RDWR);
        close(socketfd);
		exit(1);
	}
    else 
    {
        printf("Connected to the server: %s\n", he->h_name);
    }

    printf("Connect success!\n");

    char name[STRMAX] = {};

    // Get name from input argument
    strncpy(name, argv[1], STRMAX);

    // Send user name to the server
    ret = send(socketfd, name, strlen(name), 0);
    if(ret < 0)
    {
        perror("error connect");
        printf("%s\n", strerror(errno));
        exit(1);
    } 
    else 
    {
        printf("'%s', name sent to server\n", name);
    }

    // Define threads
    pthread_t tid1, tid2;

    // Create recieve pthread
    pthread_create(&tid1, NULL, recvsocket, &socketfd);
        
    // Create send pthread
    pthread_create(&tid2, NULL, sendsocket, &socketfd);
    
    printf("Thread created!\n");
    
    while(1)  
    {   
        printf("Enter your message: ");
        scanf("%s", sendbuf);

        // Exit option if user input 'exit'
        if(strcmp(sendbuf, "exit") == 0)
        {
            memset(sendbuf,0,sizeof(sendbuf));
            printf("%s, You have exit the server\n", argv[1]);
            send(socketfd,sendbuf,(strlen(sendbuf)),0);
            break;
        } 

        // Send data to server
        ret = send(socketfd, sendbuf, strlen(sendbuf), 0);
        if(ret < 0)
        {
            perror("send failed");
            return -1;
        }

        // Receive a message from the server
        ret = recv(socketfd, recvbuf, strlen(recvbuf), 0);
        if (ret < 0)
        {
            perror("recieve failed");
            exit(1);
        }

        printf("Data received: %s\n", recvbuf);

    }

    shutdown(socketfd, SHUT_RDWR);
	close(socketfd); // close socket

    pthread_cancel(tid1);
    pthread_cancel(tid2);
	exit(1);
}