#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sc.h"

#define RUNNING_DIR  "./"
#define LOCK_FILE    "servlock"

int daemonize();
void signal_handler(int );

static int lockFd;
static FILE *log;

int main(int argc, char **argv)
{
	pid_t p;
	int listenfd, connfd, readRet;
	socklen_t len;
	struct sockaddr_in servaddr, cliaddr;
	char addStr[255 + 1];
	char msg[MAXLINE], *endLine;

	if(argc != 2){
		fprintf(stderr, "%s PORT\n", argv[0]);
		return(1);
	}

	if((log = fopen("./log.text", "wr")) == NULL){
		fprintf(stderr, "Unable to open/create log.text! [%s]", strerror(errno));
		return(1);
	}
	setbuf(log, NULL);

	fprintf(log, "StartUp!\n");

	if(daemonize() == -1){
		fprintf(log, "Cannt daemonize server\n");
		return(1);
	}

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(atoi(argv[1]));

	if(bind(listenfd, (const struct sockaddr *) &servaddr, sizeof(servaddr)) != 0){
		fprintf(log, "Erro bind: [%s]\n", strerror(errno));
		return(1);
	}

	if(listen(listenfd, 1024) != 0){
		fprintf(log, "Erro listen: [%s]\n", strerror(errno));
		return(1);
	}

	for(;;){
		len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len);
		if(connfd == -1){
			fprintf(log, "Erro accept: [%s]\n", strerror(errno));
			return(1);
		}

		fprintf(log, "Connection from %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, addStr, sizeof(addStr)), ntohs(cliaddr.sin_port));
		p = fork();

		if(p == 0){ /* child */
			while(1){
				memset(msg, 0, MAXLINE);

				readRet = recv(connfd, msg, MAXLINE, 0);
				if(readRet == 0){
					fprintf(log, "End of data\n");
					break;
				}

				if(readRet < 0){
					fprintf(log, "Erro recv(): [%s]\n", strerror(errno));
					break;
				}

				endLine = strrchr(msg, '\r');
				if(endLine != NULL) (*endLine) = '\0';

				if(strncmp((char *)msg, "exit", 4) == 0) break; /* while() */

				fprintf(log, "msg: [%s]\n", msg);
			}

			close(connfd);
			break; /* for() */

		}else if(p == -1)
			fprintf(log, "Erro fork: [%s]\n", strerror(errno));

		close(connfd);
	}

	fclose(log);

	return(0);
}

int daemonize(void)
{
	pid_t i = 0, father = 0;
	char str[10];

	father = getppid();
	if(father == 1)
		return(-1);

	i = fork();

	if(i == -1)
		return(-1);

	if(i > 0)
		exit(1); /* Pai termina */

	/* Continuando filho... */
	setsid(); /* novo grupo */

	umask(027);

	chdir(RUNNING_DIR);

	/* Criando e travando arquivo de execucao unica */
	lockFd = open(LOCK_FILE, O_RDWR|O_CREAT|O_EXCL, 0640);
	if(lockFd == -1){
		fprintf(log, "There is another instance already running\n");
		exit(-1);
	}

	if(lockf(lockFd, F_TLOCK, 0) < 0){
		fprintf(log, "Cannt test 'only one instance' file\n");
		return(-1);
	}

	if(lockf(lockFd, F_LOCK, 0) < 0){
		fprintf(log, "Cannt lock 'only one instance' file\n");
		return(-1);
	}

	/* Primeira instancia */
	sprintf(str, "%d\n", getpid());
	write(lockFd, str, strlen(str));

	/* Configurando sinais */
	signal(SIGCHLD, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGHUP, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGPIPE, signal_handler);

	return(1);
}

void signal_handler(int sig)
{
	/* Qualquer sinal recebido ira terminar o server */

	fprintf(log, "Got signal [%d]!\n", sig);

	switch(sig){
		case SIGHUP:
			break;
		case SIGTERM:
			break;
	}

	if(lockf(lockFd, F_ULOCK, 0) < 0){
		fprintf(log, "Cannt unlock 'only one instance' file\n");
		exit(-1);
	}

	close(lockFd);
	if(unlink(LOCK_FILE) != 0){
		fprintf(log, "Erro deleting lock file [%s].\n", LOCK_FILE);
	}

	/* Termina servidor */
	exit(0);

	return;
}
