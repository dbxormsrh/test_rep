#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>

#define w 352
#define h 288
#define FRAME_NUM 2000
#define BUFF_SIZE 1024

ssize_t readn(int fd, void *vptr, size_t n);
void getmsec();

void fork_serv(struct sockaddr_in server_addr, struct sockaddr_in client_addr, int server_sock){
    unsigned int caddr_len = sizeof(client_addr);
    int client_sock, pid;
    while(1){
        caddr_len = sizeof(client_addr);
        if((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &caddr_len)) < 0){
            perror("server : accept");
            return -1;
        }
        printf("Echo request from %s\n", inet_ntoa(client_addr.sin_addr));
        printf("client_sock = %d\n", client_sock);
        if((pid = fork()) < 0){
                perror("fork() failed");
                exit(1);
        }
        if(pid == 0){
            close(server_sock);
            recv_file(client_sock);
            exit(0);
        }
        printf("Process %d\n", getpid()); 
        close(client_sock);
    }
}

char* getCID(){
	time_t     tm_time;
	struct tm* st_time;
    
	time(&tm_time);
	st_time = localtime(&tm_time);
    static char date[BUFF_SIZE];
    strftime(date, BUFF_SIZE, "%Y.%m.%d-%l:%M:%S", st_time);;
	return date;
}

void recv_file(int csock){
    unsigned char buf[h * w * 3 / 2];
    FILE* file;
    char *file_name = getCID();
    strcat(file_name, ".raw");
    file = fopen(file_name, "wb");
    
    int size = 1;
    while(size != 0){
        size = recv(csock, buf, 256, 0 != 0);
        fwrite(buf, sizeof(unsigned char), size, file);
    }
    getmsec();
    fclose(file);
}

void getmsec(){
    struct timeb itb;
    struct tm *lt;
    ftime(&itb);
    lt = localtime(&itb.time);
    printf("%02d:%02d:%02d.%03d\n", lt->tm_hour, lt->tm_min, lt->tm_sec, itb.millitm);
}