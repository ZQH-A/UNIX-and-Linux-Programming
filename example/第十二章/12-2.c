#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
int main(int argc,char *argv[])
{
    char *host, **names, **addrs, myname[256];
    struct hostent *hostinfo;
    if(argc>1){
        host=argv[1];
    }else{
        gethostname(myname,255);
        host=myname;
    }
    hostinfo =gethostbyname(host);
    if(!hostinfo){
        fprintf(stderr,"cannot get info for host: %s\n",host);
        exit(1);
    }
    printf("results for host %s:\n",host);
    printf("Official name: %s\n",hostinfo->h_name);
    printf("\tAliases:");
    names =hostinfo->h_aliases;
    while(*names)
    {
        printf("%s\n",*names);
        names++;
    }
    addrs =hostinfo->h_addr_list;
    while(*addrs)
    {
        printf("\taddress:%s\n",inet_ntoa(*(struct in_addr*)*addrs));
        addrs++;
    }
    exit(0);
}
