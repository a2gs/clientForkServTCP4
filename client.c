/* Andre Augusto Giannotti Scota (a2gs)                              
 * andre.scota@gmail.com
 *
 * A C TCPIPv4 client (with DNS resolution)
 *
 * Public Domain
 *
 */

/* client.c
 *
 *  Who     | When       | What
 *  --------+------------+----------------------------
 *   a2gs   | 01/01/2005 | Creation
 *          |            |
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "sc.h"

int main(int argc, char **argv)
{
	int sockfd = 0;
	struct sockaddr_in servaddr;
	char msg[MAXLINE] = {0};
	struct hostent *servResolved = NULL;
	struct in_addr ip_addr;

	if(argc != 3){
		fprintf(stderr, "%s IP_ADDRESS PORT\n", argv[0]);
		return(-1);
	}

	signal(SIGPIPE, SIG_IGN);

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("ERRO: socket() [%s]\n", strerror(errno));
		return(-1);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(atoi(argv[2]));

	servResolved = gethostbyname(argv[1]);
	if(servResolved == NULL){

		printf("ERRO: gethostbyname() ");
		switch(h_errno){
			case HOST_NOT_FOUND:
				printf("[The specified host is unknown]\n");
				break;

			case NO_ADDRESS:
			/*case NO_DATA:*/
				printf("[The requested name is valid but does not have an IP address]\n");
				break;

			case NO_RECOVERY:
				printf("[A non-recoverable name server error occurred]\n");
				break;

			case TRY_AGAIN:
				printf("[A temporary error occurred on an authoritative name server. Try again later]\n");
				break;

			default:
				printf("[error unknow]\n");
				break;
		}
		return(-1);
	}

	ip_addr = *(struct in_addr *)(servResolved->h_addr);

	if(inet_pton(AF_INET, inet_ntoa(ip_addr), &servaddr.sin_addr) < 0){
		printf("ERRO: inet_pton() [%s]\n", strerror(errno));
		return(-1);
	}

	if(connect(sockfd, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
		printf("ERRO: connect() [%s]\n", strerror(errno));
		return(-1);
	}

	for(;;){
		memset(msg, 0, MAXLINE);

		memcpy(msg, "ok", 2);

		if(send(sockfd, msg, 2, 0) == -1){
			printf("ERRO: send() [%s]\n", strerror(errno));
			break;
		}
		sleep(2);
	}

	close(sockfd);

	return(0);
}
