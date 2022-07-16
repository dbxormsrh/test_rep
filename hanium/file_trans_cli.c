#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

char *escapechar = "exit\n";
int main(int argc, char *argv[])
{
	char sbuf[256], rbuf[256];
	char *haddr;
	int n, size, comp, port;
	pid_t pid;
	int sock;
	struct sockaddr_in servaddr;

	FILE *file = fopen("Sent_file.txt", "wb");
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

	printf("I am child, &ld &ld\n", getpid(), getppid());
	size = 256;
	int step = 1;
	while(size != 0){
		size = recv(sock, rbuf, 256, 0);
		fwrite(rbuf, sizeof(char), size, file);
		printf("step : %d, size : %d\n", step++, size);
	}
	fclose(file);
	close(sock);
}
