#include "12-7.h"
#include <sys/socket.h>
#include "err_exit.h"
#include <stdio.h>
#define MAXMSG 1024
int main()
{
    int sock,size,nbytes;
    struct sockaddr_in name;
    char message[MAXMSG];
    sock=make_socket(SOCK_DGRAM,0);
    size =sizeof(name);
    if(getsockname(sock,(struct sockaddr*)&name,(socklen_t*)&size))
        err_exit("getting socket name");
    printf("Socket has port #%d\n",ntohs(name.sin_port));
    while(1)
    {
        size=sizeof(name);
        nbytes=recvfrom(sock,message,MAXMSG,0,(struct sockaddr*)&name,(socklen_t *)&size);
        if(nbytes<0)
            err_exit("recvfrom(server)");
        printf("Seerver got message :%s\n",message);
        nbytes=sendto(sock,message,nbytes,0,(struct sockaddr *)&name,size);
        if(nbytes<0)
            err_exit("sendto (server)");        
    }
}
