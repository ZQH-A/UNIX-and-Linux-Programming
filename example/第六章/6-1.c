#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int global =5;

int main()
{
    pid_t pid;
    char *string ="I am parent";
    int local=10;
    printf("before fork---");
    if((pid=fork())<0)
        //err_exit("fork");
        return -1;
    if(pid==0){
        string ="I am child";
        printf("%s, my pid =%d: global=%d, local=%d\n",string,getpid(),global,local);
        global++;
    }else{
        printf("%s, my pid =%d: global=%d, local=%d\n",string,getpid(),global,local);
        local++;
    }
    printf("At join point, %s: global=%d, local=%d\n",string ,global,local);
    exit(EXIT_SUCCESS);
}
