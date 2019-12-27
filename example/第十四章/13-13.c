#include <pthread.h>
#include "ch13.h"
#include <errno.h>
typedef struct barrier_struct{
    int valid;
    pthread_cond_t cv;
    pthread_mutex_t mtx;
    int predicate;
    int barrier_val;
    int blocked_threads;
}barrier_t;

pthread_mutex_t barrier_init_mutex =PTHREAD_MUTEX_INITIALIZER;
#define BARRIER_VALID 546731
int barrier_init(barrier_t *b,int val)
{
    int rv;
    if((rv=pthread_mutex_lock(&barrier_init_mutex))!=0)
        return (rv);
    if(b->valid==BARRIER_VALID)
    {
        if((rv=pthread_mutex_lock(&b->mtx))!=0)
        {
            pthread_mutex_unlock(&barrier_init_mutex);
            return (rv);
        }
        if(b->blocked_threads!=0){
            pthread_mutex_unlock(&b->mtx);
            pthread_mutex_unlock(&barrier_init_mutex);
            return(EBUSY);
        }
        b->barrier_val=val;
        if((rv=pthread_mutex_unlock(&b->mtx))!=0){
            pthread_mutex_unlock(&barrier_init_mutex);
            return(rv);
        }
    }else{
        if((rv=pthread_mutex_init(&b->mtx,NULL))!=0)
        {
            return(rv);
        }
        if((rv=pthread_cond_init(&b->cv,NULL))!=0)
        {
            pthread_mutex_unlock(&barrier_init_mutex);
            return (rv);
        }
        b->barrier_val=val;
        b->blocked_threads=0;
        b->predicate=0;
        b->valid=BARRIER_VALID;
    }
    if((rv=pthread_mutex_unlock(&barrier_init_mutex))!=0)
        return (rv);
    return 0;
}

int barrier_wait(barrier_t *b)
{
    int rv,predicate;
    if(b->valid!=BARRIER_VALID)
        return (EINVAL);
    if((rv=pthread_mutex_lock(&b->mtx))!=0)
        return (rv);
    predicate=b->predicate;
    b->blocked_threads++;
    if(b->blocked_threads==b->barrier_val)
    {
        b->predicate+=1;
        b->blocked_threads=0;
        if((rv=pthread_cond_broadcast(&b->cv))!=0){
            pthread_mutex_unlock(&b->mtx);
            return (rv);
        }
    }else{
        while(b->predicate==predicate){
            rv=pthread_cond_wait(&b->cv,&b->mtx);
            if((rv!=0)&&(rv!=EINTR))
            {
                pthread_mutex_unlock(&b->mtx);
                return (rv);
            }
        }
    }
    if((rv=pthread_mutex_unlock(&b->mtx))!=0)
        return (rv);
    return 0;
}
