#include "12-7.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#define LISTENQ 5
#define MAXLINE 512
int main(int argc,char **argv)
{
    int listenfd,connfd;
    socklen_t len;
    struct sockaddr_in servaddr,cliaddr ;
    char buf[MAXLINE];
    time_t ticks;
    listenfd=make_socket(SOCK_STREAM,9001);
    listen(listenfd,LISTENQ);
    while(1){
        len=sizeof(cliaddr);
        connfd=accept(listenfd,(struct sockaddr *)&cliaddr,&len);
        printf("connect from %s,port %d\n",inet_ntoa((struct in_addr)cliaddr.sin_addr),ntohs(cliaddr.sin_port));
        ticks=time(NULL);
        sprintf(buf,"%s.24s\r\n",ctime(&ticks));
        write(connfd,buf,strlen(buf));
        close(connfd);
    }
}
