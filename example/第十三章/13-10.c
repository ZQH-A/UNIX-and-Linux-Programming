#include "ch13.h"
#include <pthread.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
typedef struct sync_struct{
    pthread_cond_t cond;
    pthread_mutex_t mtx;
    int pred;
}sync_struct_t;
#define KEY 1234
void create_pshared_cond_sync_data(sync_struct_t *sync_data)
{
    pthread_mutexattr_t mtx_attr;
    pthread_condattr_t cond_attr;
    int rv,shmid;
    rv=pthread_mutexattr_init(&mtx_attr);
    check_error(rv,"mutexattr_init");
    rv=pthread_condattr_init(&cond_attr);
    check_error(rv,"condattr_init");
    rv=pthread_mutexattr_setpshared(&mtx_attr,PTHREAD_PROCESS_SHARED);
    check_error(rv,"mutexattr_setpshared");
    
    rv=pthread_condattr_setpshared(&cond_attr,PTHREAD_PROCESS_SHARED);\
    check_error(rv,"condattr_setpshared");
    
    if((shmid=shmget(KEY,sizeof(sync_struct_t),066|IPC_CREAT))<0)
        err_exit("shmget");
    if((sync_data=(sync_struct_t *)shmat(shmid,NULL,0))==(sync_struct_t *)-1)
        err_exit("shmat");
    
    rv=pthread_mutex_init(&sync_data->mtx,&mtx_attr);
    check_error(rv,"mutex_init");
    rv=pthread_cond_init(&sync_data->cond,&cond_attr);
    check_error(rv,"cond_init");
    
    rv=pthread_mutexattr_destroy(&mtx_attr);
    check_error(rv,"mutexattr_destroy");
    rv=pthread_condattr_destroy(&cond_attr);
    check_error(rv,"condattr_destroy");
    return;
}
