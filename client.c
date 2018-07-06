/*client.c
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef enum {False = 0, True = 1} bool;

int open_clientfd(char *hostname, int port)
{
    int clientfd;
    struct hostent *hp;
    struct sockaddr_in serveraddr;

    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1; /*check errno for cause of error*/

    /*fill in server's IP Address and port*/
    if ((hp = gethostbyname(hostname)) == NULL)
	{	
		printf("ERROR: gethostbyname() failed)");
        	return -2; /*check errno for error*/
	}
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)hp->h_addr_list[0],
          (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
    serveraddr.sin_port = htons(port);

    /*establish a connection with server*/
    if(connect(clientfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
    {	
	printf("ERROR: failed to connect with server");
	return -1;

	}
    return clientfd;
}

int main(int argc, char * argv[]){
    int n, c;
	bool done = False;
	if(argc != 3){
		perror("Error: must include host name and port number");
		exit(0);
	}

    char * host_name = argv[1];
	int port_num = atoi(argv[2]);
	int clientfd;

	char buf[255];
    while (done == False) {	
     		int i;
		printf(">");
		bzero(buf, 255);
		scanf("%s", buf);
		
	
		
		if(strcmp(buf,"exit") == 0){
            done = True;
			puts("Client has exited.");
			exit(0);
        }
		else{	
			clientfd = open_clientfd(host_name, port_num);
			if( (c = write(clientfd, buf, 255)) < 0)
				perror("Error writing to socket"); 
			/*
			Clear the buffer in order to read it the message as a whole. 
			*/
			bzero(buf, 255);
			if( ( n = read( clientfd, buf, 255)) < 0)
				perror("Error reading from socket");
			
			printf("%s\n", buf);
			close(clientfd);
			
    	}


	}


	exit(0);
}
