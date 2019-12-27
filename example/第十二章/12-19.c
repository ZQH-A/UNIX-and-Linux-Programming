#include "12-6.h"
#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include "err_exit.h"
#define MAXMSG 512
int main(int argc,char **argv)
{
    int sock,n;
    struct sockaddr_in name;
    struct hostent *hp;
    
    char sndmsg[MAXMSG],recvmsg[MAXMSG];
    if(argc<3){
        fprintf(stderr,"Usage:a.out <hostname> <port>\n");
        exit(1);
        
    }
    if((sock=socket(AF_INET,SOCK_DGRAM,0))<0)
        err_exit("opening datagram socket error");
    init_sockaddr(&name,argv[1],argv[2]);
    while(1){
        printf("Please enter any string:");
        fgets(sndmsg,MAXMSG,stdin);
        if(sndmsg[0]=='\n')
            break;
        if(sendto(sock,sndmsg,sizeof(sndmsg),0,(struct sockaddr *)&name,sizeof(name))<0)
            err_exit("(client)sending error");
        if((n=recvfrom(sock,recvmsg,MAXMSG,0,NULL,NULL))<0)
            err_exit("(client)receive error");
        recvmsg[n]=0;
        printf("I recvived echo: %s",recvmsg);
    }
    close(sock);
    exit(0);  
}
