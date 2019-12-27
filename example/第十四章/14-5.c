#include <pthread.h>
#include <stdio.h>
#include "ch13.h"
#include <stdlib.h>
#include <unistd.h>
typedef struct Sema{
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int count;
}sema_t;
sema_t semaphore={PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER,1};
#define THREADS 4
void cleanup_mutex(void *arg)
{
    printf("\tthread %lx:cleanup/unlock_mutex\n",pthread_self());
    pthread_mutex_unlock((pthread_mutex_t*)arg);
}
void AquireSemaphore(sema_t *ps)
{
    pthread_mutex_lock(&ps->lock);
    pthread_cleanup_push(cleanup_mutex,&ps->lock);
    while(ps->count==0)
        pthread_cond_wait(&(ps->cond),&(ps->lock));
    --ps->count;
    pthread_cleanup_pop(1);
}
void ReleaseSemaphore(sema_t *ps)
{
    pthread_mutex_lock(&ps->lock);
    ++ps->count;
    pthread_cond_signal(&ps->cond);
    pthread_mutex_unlock(&ps->lock);
}
void cleanup_sema(void *arg)
{
    printf("\t thread %lx:cleanup_sema\n",pthread_self());
    ReleaseSemaphore((sema_t*)arg);
}
void *thread_routine(void *arg)
{
    int rv;
    AquireSemaphore(&semaphore);
    pthread_cleanup_push(cleanup_sema,&semaphore);
    printf("thread %lx:sleep\n",pthread_self());
    sleep(1);
    printf("thread %lx:waken from sleep\n",pthread_self());
    pthread_cleanup_pop(0);
    ReleaseSemaphore(&semaphore);
}
int main(int argc,char *argv[])
{
    pthread_t thread_id[THREADS];
    int i,rv,*status;
    for(i=0;i<THREADS;i++)
    {
        rv=pthread_create(&thread_id[i],NULL,thread_routine,NULL);
        check_error(rv,"Create thread");
    }
    sleep(2);
    for(i=0;i<THREADS;i++)
    {
        if((rv=pthread_cancel(thread_id[i]))=ESRCH)
            printf("thread %lx dose not exist\n",thread_id[i]);
        rv=pthread_join(thread_id[i],(void **)&status);
        check_error(rv,"Join thread");
        if(status==PTHREAD_CANCELED)
            printf("thread %d cancel %lx\n",i,thread_id[i]);
        else
            printf("thread %d was not canceled\n",i);
    }
    return 0;
}
