#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <wait.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "test.c"

int main(int argc, char *argv[])
{
    int pid, server_sock, client_sock;
	struct sockaddr_in server_addr, client_addr;
    unsigned short server_port;
    unsigned int caddr_len;
    char* CID;

	if(argc != 2){
		printf("Usage : %s TCP_PORT\n", argv[0]);
		return -1;
	}
    server_port = atoi(argv[1]);
	if((server_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		perror("server : socket");
		return -1;
	}

	memset(&server_addr, 0, sizeof(server_addr));           	// --> memset() 하는 이유? sockaddr_in은 sin_zero라는 8bytes의 dummy data가 있다. 이 녀석은 16bytes인 struct sockaddr과 크기를 일치시키기 위한 목적.  이녀석은 반드시 0으로 채워져있어야 함으로 memset을 통해 0으로 할당함.

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(server_port);

	if(bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		perror("server : bind");
		return -1;
	}

	if(listen(server_sock, 5) < 0){
        perror("listen() failed");
        exit(1);
    	}
    printf("fork_server start!\n");
    fork_serv(server_addr, client_addr, server_sock);
}