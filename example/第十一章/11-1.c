#include <unistd.h>
#include <sys/types.h>
#include "err_exit.h"
#include <stdio.h>
#include <string.h>
int main()
{
    pid_t pid;
    int n,mypipe[2];    
    char buffer[BUFSIZ+1],some_data[]="Hello, world!"; 
    if(pipe (mypipe))
    {
        err_exit("Pipe failde.\n");
    }
    if((pid=fork())==(pid_t)0){
        close(mypipe[1]);
        n=read(mypipe[0],buffer,BUFSIZ);
        printf("child %d:read %d bytes: %s\n",getpid(),n,buffer);   
        
    }else{
        close(mypipe[0]);
        n=write(mypipe[1],some_data,strlen(some_data));
        printf("parent %d:write %d bytes: %s \n",getpid(),n,some_data);
    }
    exit(EXIT_SUCCESS);
}
