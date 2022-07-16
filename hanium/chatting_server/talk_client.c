#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char *escapechar = "exit\n";
int main(int argc, char *argv[])
{
	char sbuf[BUFSIZ], rbuf[BUFSIZ+1];
	char *haddr;
	int n, size, comp, port;
	pid_t pid;
	int sock;
	struct sockaddr_in servaddr;

	if(argc != 3){
		printf("Usage : %s SERVER_ADDRESS TCP_PORT\n", argv[0]);
		return -1;
	}
	else{
		haddr = argv[1];
		port = atoi(argv[2]);
	}

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("client : socket");
		return -1;
	}

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(haddr);
	servaddr.sin_port = htons(port);

	if(connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		perror("client : connect");
		return -1;
	}
	printf("Connected to server...\n");

	if((pid = fork()) > 0){
		printf("I am parent, &d &d\n", getpid(), getppid());
		while(fgets(sbuf, BUFSIZ, stdin) != 0){
			if(strlen(sbuf) == 5 && strncmp(sbuf, escapechar, 4) == 0){
				kill(pid, SIGQUIT);
				exit(1);
			}

			size = strlen(sbuf);

			if(write(sock, sbuf, strlen(sbuf)) != size){
				perror("client : write");
				return -1;
			}
		}
	}
	else if(pid == 0){
		printf("I am child, &d &d\n", getpid(), getppid());
		while(1){
			if((size = read(sock, rbuf, BUFSIZ)) > 0){
				rbuf[size] = '\0';
				if(size == 5 && strncmp(rbuf, escapechar, 4) == 0){
					exit(1);
				}
				printf("%s", rbuf);
			}
		}
	}
	close(sock);
}
