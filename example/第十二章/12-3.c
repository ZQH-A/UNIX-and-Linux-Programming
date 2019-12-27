#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
int main()
{
    int stayopen=1;
    struct servent*sp;
    setservent(stayopen);
    while(1)
    {
        sp=getservent();
        if(sp!=(struct servent*)0){
            printf("servent name=%12s,port=%6d,proto=%4s\n",sp->s_name,ntohs(sp->s_port),sp->s_proto);//ntohs()将网络字节顺序转换为主机字节顺序
        }else{
            break;
        }
    }
    endservent();
    sp=getservbyname("telnet","tcp");
    if(sp!=(struct servent*)0)
        printf("telnet's port is %d\n",ntohs(sp->s_port));
    else
        printf("ERROR: getservbyname calle failed\n");
    return 0;
}
