#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char *escapechar = "exit\n";
int main(int argc, char *argv[])
{
	int servfd, clifd, clilen, num, size;
	char sbuf[BUFSIZ], rbuf[BUFSIZ];
	pid_t pid;
	struct sockaddr_in cliaddr, servaddr;

	if(argc != 2){
		printf("Usage : %s TCP_PORT\n", argv[0]);
		return -1;
	}

	if((servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("server : socket");
		return -1;
	}

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	if(bind(servfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		perror("server : bind");
		return -1;
	}

	printf("Server started.\n Waiting for client...\n");
	listen(servfd, 5);
	clilen = sizeof(cliaddr);

	if((clifd = accept(servfd, (struct sockaddr *)&cliaddr, &clilen)) != -1){
		printf("Server : client connected.\n");
	}
	else{
		perror("server : accept");
		return -1;
	}

	if((pid = fork()) > 0){
		while(fgets(sbuf, BUFSIZ, stdin) != 0){
			size = strlen(sbuf);
			if(write(clifd, sbuf, size) != size){
				perror("server : write");
				return -1;
			}
			if(strlen(sbuf) == 5 && strncmp(sbuf, escapechar, 4) == 0){
				kill(pid, SIGQUIT);
				exit(1);
			}
		}
	}
	else if(pid == 0){
		while(1){
			if((size = read(clifd, rbuf, BUFSIZ)) > 0){
				rbuf[size] = '\0';
				if(strncmp(rbuf, escapechar, 4) == 0){
					break;
				}
				printf("%s", rbuf);
			}
		}
	}
	close(servfd);
	close(clifd);
}
