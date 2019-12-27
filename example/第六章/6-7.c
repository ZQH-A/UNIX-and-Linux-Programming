#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    if((pid=fork())<0)
        return -1;
    else if(pid==0){
        if((pid==fork())<0)
            return -1;
        else if(pid>0){
            printf("child exit\n");
            //printf("grandchild's parent pid=%d\n",getppid());
            exit("EXIT_SUCCESS");
        }        
        sleep(2);
        printf("grandchild's parent pid=%d\n",getppid());
        printf("child's parent pid=%d\n",getpid());
        exit(EXIT_SUCCESS);
    }
    if(waitpid(pid,NULL,0)!=pid)
        return -1;
    
    printf("parent exit,%d\n",getpid());
    exit(EXIT_SUCCESS);
}
