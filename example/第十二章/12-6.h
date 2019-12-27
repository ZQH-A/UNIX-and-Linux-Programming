#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
void init_sockaddr(struct sockaddr_in *name,const char *hostname,const char *serv)//参数serv是端口号
{
    struct hostent *hp;
    char *host, myname[255];
    if(hostname==NULL){
        gethostname(myname,sizeof(myname));
        host =myname;
    }else{
        host =hostname;
    }
    if((hp=gethostbyname(host))==NULL)//得到主机的地址信息
    {
        printf("Unknown host: %s\n",host);
        exit(-1);
    }
    bzero(name,sizeof(struct sockaddr));//将name空间清零
    if(hp->h_addrtype ==AF_INET)
    {
        name->sin_family =AF_INET;
        bcopy(hp->h_addr_list[0],&name->sin_addr,hp->h_length);
        if(serv==NULL)
            name->sin_port=htons(0);//由系统选择合适的端口号
        else
            name->sin_port=htons(atoi(serv));//atoi（）将字符数组转换为整数
    }else{
        printf("Unknown address type\n");
        exit(-1);
    }     
}
