#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "test.c"

#define w 352
#define h 288
#define FRAME_NUM 2000

int main(int argc, char *argv[])
{
	unsigned char rbuf[h * w * 3 / 2] = { 0, };
	char *haddr;
	int size,  port;
	pid_t pid;
	int sock;
	struct sockaddr_in servaddr;

	FILE *file = fopen("bridge-close_cif.yuv", "rb");

    if(file == NULL){
        fputs("File read Fail", stderr);
        exit(1);
    }

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
	getmsec();
	int y_frame = h * w;
	int fsize = 0;
	for (int f = 0; f < FRAME_NUM; f++){
		fread(rbuf, 1, y_frame, file);
		send(sock, rbuf, y_frame, 0);
		fseek(file, h * w / 2, SEEK_CUR);
	}

	fclose(file);
	close(sock);
}

void get_video(){

}
