#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <wait.h>
#include <sys/file.h>
#include <arpa/inet.h>
#include "sock.h"

int main(int argc, char *argv[])
{
	if(argc != 2){
		printf("Usage : %s TCP_PORT\n", argv[0]);
		return -1;
	}
	
	SOCKET sock(atoi(argv[1]));
	printf("create socket\n");
	sock.sock_bind();
	printf("bind socket\n");
	sock.sock_listen();
	printf("listen socket\n");
	sock.sock_accept();
	printf("accept socket\n");
	sock.sock_recv();
	printf("recv socket\n");

}
