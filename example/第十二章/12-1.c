#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include <sys/types.h>
#include "err_exit.h"
#define DATA1 "Fine,thanks."
#define DATA2 "Hello,how are you?"
int main()
{
    int sockets[2],child;
    char buf[1024];
    if(socketpair(AF_UNIX,SOCK_STREAM,0,sockets)<0)
        err_exit("socketpait error");
    if((child =fork())==-1)
        err_exit("fork error");
    if(child!=0){  //父进程
        close(sockets[0]);
        if(read(sockets[1],buf,sizeof(buf))<0)
            err_exit("reading socket error parent");
        printf("parent %d received request :%s\n",getpid(),buf);
        if(write(sockets[1],DATA1,sizeof(DATA1))<0)
            err_exit("writing socket error");
        close(sockets[1]);        
    }else{ //子进程
        close(sockets[1]);
        if(write(sockets[0],DATA2,sizeof(DATA2))<0)
            err_exit("writing socket error");
        if(read(sockets[0],buf,sizeof(buf))<0)
            err_exit("reading socket error son");
        printf("child process %d received answer:%s\n",getpid(),buf);
        close(sockets[0]);
    }
}
