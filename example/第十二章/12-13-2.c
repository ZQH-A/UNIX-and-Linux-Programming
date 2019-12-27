#include "12-8.h"
#include <unistd.h>
int main(int argc,char **argv)
{
    int connfd,n,result;
    char buf[256];
    connfd=socket_connect(NULL,"2003");
    write(connfd,argv[1],sizeof(argv[1]));
    n=read(connfd,buf,sizeof(buf));
    buf[n]=0;
    printf("string from server=%s\n",buf);
    close(connfd);
    exit(0);
}
