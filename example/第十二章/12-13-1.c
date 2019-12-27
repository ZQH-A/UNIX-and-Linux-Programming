#include "12-7.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
void doit(int);
int main()
{
    int server_sockfd,client_sockfd;
    int server_len,client_len;
    struct sockaddr_in client_address;
    server_sockfd=make_socket(SOCK_STREAM,2003);
    listen(server_sockfd,5);
    signal(SIGCHLD,SIG_IGN);//忽略SIGCHLD（子进程终止）信号
    while(1)
    {
        printf("server waiting\n");
        client_len=sizeof(client_address);
        client_sockfd=accept(server_sockfd,(struct sockaddr*)&client_address,&client_len);
        if(fork()==0){//子进程
            close(server_sockfd);
            doit(client_sockfd);
            close(client_sockfd);
            exit(0);
        }else{
            close(client_sockfd);
        }
    }
}
void doit(client_sockfd)
{
    char buf[255];
    int n;
    n=read(client_sockfd,buf,sizeof(buf));
    buf[n]=0;
    sleep(1);
    write(client_sockfd,buf,n);
}
