#include "ch13.h"
#include <sys/shm.h>
#include <stdlib.h>
#include "err_exit.h"
#include <pthread.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h> 
#include "xmalloc.c"
pthread_mutex_t *mutex_customized(int shared,int type,int key)
{
    int rv;
    key_t shmid;
    pthread_mutex_t *mtx;
    pthread_mutexattr_t mtxattr;
    
    rv=pthread_mutexattr_init(&mtxattr);
    check_error(rv,"mutexattr_init");
    rv=pthread_mutexattr_setpshared(&mtxattr,shared);
    check_error(rv,"mutexattr_setpshared");
    
    rv=pthread_mutexattr_settype(&mtxattr,type);
    check_error(rv,"mutexattr_settype");
    
    if(shared==PTHREAD_PROCESS_PRIVATE)
        mtx=(pthread_mutex_t *)xmalloc(sizeof(pthread_mutex_t));
    else{
        shmid =shmget(key,sizeof(pthread_mutex_t),0666|IPC_CREAT);
        if(shmid==-1)
            err_exit("shmget error");
        mtx=(pthread_mutex_t *)shmat(shmid,(char *)0,0);
        if((int)mtx==-1)
            err_exit("shmget error");
    }
    rv=pthread_mutex_init(mtx,&mtxattr);
    check_error(rv,"mutex_init");
    rv=pthread_mutexattr_destroy(&mtxattr);
    check_error(rv,"mutexattr destroy");
    return (mtx);
}

#define KEY 8125
pthread_mutex_t mtx1=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx2;
pthread_mutex_t *mtx3;
int main()
{
    int rv;
    rv=pthread_mutex_init(&mtx2,NULL);
    mtx3=mutex_customized(PTHREAD_PROCESS_SHARED,PTHREAD_MUTEX_RECURSIVE,KEY);
    
    rv=pthread_mutex_destroy(&mtx2);
    check_error(rv,"mutex_destroy");
    pthread_exit(NULL);
}
