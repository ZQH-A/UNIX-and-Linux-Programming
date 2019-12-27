#include "12-6.h"
#include "err_exit.h"
#include <sys/socket.h>
int socket_connect(char *hostname,char *serv_port)
{
    int sockfd;
    struct sockaddr_in saddr;
    struct hostent *hp;
    char *host,myname[104];
    init_sockaddr(&saddr,hostname,serv_port);
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
        err_exit("socket error");
    if(connect(sockfd,(struct sockaddr *)&saddr,sizeof(saddr))<0)
        err_exit("connect error");
    return (sockfd);
}
