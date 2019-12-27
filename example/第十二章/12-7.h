#include <netinet/in.h>
#include "err_exit.h"
#include <stdio.h>
#include <arpa/inet.h> 
int make_socket(int type,unsigned short int port)
{
    int sock;
    struct sockaddr_in name;
    if((sock=socket(AF_INET,type,0))<0)
        err_exit("socket");
    name.sin_family=AF_INET;
    name.sin_port=htons(port);
    name.sin_addr.s_addr=htonl(INADDR_ANY);
     //if (inet_aton("127.0.0.1", &(name.sin_addr)) != 1)
    //{
    //    fprintf(stderr, "inet_aton error!!!\n");
    //    exit(1);
   // }
    if(bind(sock,(struct sockaddr *)&name,(socklen_t)sizeof(name))<0)
        err_exit("blind");
    return sock; 
}
