#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "getmsec.h"

char *escapechar = "exit\n";
int main(int argc, char *argv[])
{
	int servfd, clifd, clilen, num;
	char *sbuf, *rbuf, *tmp;
	pid_t pid;
	struct sockaddr_in cliaddr, servaddr;

	FILE *file;
	size_t fsize, nsize = 0;

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

	file = fopen("dummy_file.txt", "rb");
	
	if(file == NULL){
		fputs("File error", stderr);
		exit(1);
	}

	fseek(file, 0, SEEK_END);
	fsize = ftell(file);
	rewind(file);
	sbuf = (char*)malloc(sizeof(char)* fsize);
	printf("file_size : %d\n", fsize);
	tmp = sbuf;
	if(sbuf == NULL){
		fputs("Memory error", stderr);
		exit(2);
	};

	printf("Server started.\nWaiting for client...\n");
	listen(servfd, 5);
	//-> server에서 listen 중 client에서의 connect 요청이 오면 처리가 어떻게 되는지?

	clilen = sizeof(cliaddr);
	if((clifd = accept(servfd, (struct sockaddr *)&cliaddr, &clilen)) != -1){
		printf("clifd : %d\n", clifd);
		printf("Server : client connected.\n");
	}
	else{
		perror("server : accept");
		return -1;
	}

	getmsec();

	while(nsize != fsize){
		int fpsize = fread(sbuf, 1, 256, file);
		nsize += fpsize;
		send(clifd, sbuf, fpsize, 0);
	}
	sbuf = tmp;
	free(sbuf);
	fclose(file);
	close(servfd);
	close(clifd);
	printf("\ncompletly operate\n");
}