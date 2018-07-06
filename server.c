/*
Server.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <ctype.h>

int open_listenfd(int port){
	int listenfd;
	int optval = 1;

	struct sockaddr_in serveraddr;
	
	/*create a socet descriptor */	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd < 0) {
		return -1;
	}
	/* Eliminates "address already in use" error from bind */
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *) &optval, sizeof(int)) < 0)
	{
		printf("Error in setsockopt");
		return -1;
	}

	/* Listenfd will be an endpoint for all requests to port on any IP address for this host   */	
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)  port);
	
	if( bind(listenfd, (struct sockaddr* )& serveraddr, sizeof(serveraddr)) < 0)
	{	
		printf("Error in bind");
		return -1;		
	}
	if(listen(listenfd, 5) < 0){
		printf("Error in listen");
		return -1;
	}
	return listenfd;
}



int main(int argc , char * argv[]){
	int listenfd , connfd, port, clientlen, n;
	struct sockaddr_in clientaddr;
	struct hostent *hp;
	char *haddrp;
	unsigned short client_port;
	char buffer[255];
	char buffer2[255];
	/*port number that will be passed on commandline*/
	port = atoi(argv[1]);

	listenfd = open_listenfd(port);
	printf("Address server started\n");
	
	while(1){
		int i;
		char response[255] ;
		strcpy(response, "unknown");
		clientlen = sizeof(clientaddr);
		connfd = accept(listenfd, (struct sockaddr * ) &clientaddr, &clientlen);
		if(connfd < 0){printf("Error on accept");exit(0);	}	
		bzero(buffer, 255);
		
		if(( n = read(connfd, buffer, 255)) < 0)
			perror("Error on read");
		printf("message recieved: %s\n", buffer);
		fflush(stdout);
	

		for(i = 0; i < strlen(buffer); i++){
			buffer[i] = toupper(buffer[i]);
		}		
		if((n = write(connfd, buffer, 255)) < 0){
			perror("Error on write");
			exit(0);
		}
	}
	
	return 0; 
}
