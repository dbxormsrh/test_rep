#include <sys/socket.h>
#include <sys/file.h>
#include "test.c"

#define BUF_SIZE 152064

class SOCKET{
    struct sockaddr_in sock_addr, client_addr;
    int sock_fd, client_fd;
    long file_size = 352 * 288 * 3 / 2;
    unsigned char *buf;
public:
    SOCKET(char port);
    SOCKET(char address, char port);
    void sock_bind();
    void sock_accept();
    void sock_listen();
    void sock_connect();
    void sock_recv();
    void sock_send();
    void set_file_size(long size);
};

SOCKET::SOCKET(char port){
    buf = new unsigned char[file_size];

    if((sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        perror("Failed to open socket");
        exit(1);
    }

    int opt = file_size;
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, &opt, sizeof(opt));
    setsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, &opt, sizeof(opt));

    opt = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(port);
}

SOCKET::SOCKET(char address, char port){
    buf =  new unsigned char[file_size];

    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Failed to open socket");
        exit(1);
    }
    int opt = file_size;
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, &opt, sizeof(opt));
    setsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, &opt, sizeof(opt));

    opt = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(address);
    sock_addr.sin_port = htons(port);
}

void SOCKET::sock_bind(){
    if(bind(sock_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0){
        perror("Failed to bind socket");
        exit(1);
    }
}

void SOCKET::sock_accept(){
    unsigned int caddr_len = sizeof(client_addr);
    while(1){
        caddr_len = sizeof(client_addr);
        if((client_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &caddr_len)) < 0){
            perror("server : accept");
        }
        else{
            sock_recv();
        }
        close(client_fd);
    }
}

void SOCKET::sock_listen(){
    if(listen(sock_fd, 5) <= 0){
        perror("Failed listen()");
        exit(1);
    }
}

void SOCKET::sock_recv(){
    unsigned char buf[file_size];
    FILE* file;
    char *file_name = getCID();
    strcat(file_name, ".raw");
    file = fopen(file_name, "wb");
    

    int size = 1;
    while(size != 0){
        size = recv(client_fd, buf, 256, 0 );    
    }
    fwrite(buf, sizeof(unsigned char), size, file);
    getmsec();
    fclose(file);
}

void SOCKET::set_file_size(long size){
    file_size = size;
}