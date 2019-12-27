#include <stdio.h>
#include "ch13.h"
#include <pthread.h>
#define SINGLE_PROCESSOR  1
#define MAX_SPIN  2000
#define my_spinlock_init pthread_mutex_init
#define my_spinunlock pthread_mutex_unlock
#define my_spintrylock pthread_mutex_trylock

int my_spinlock(pthread_mutex_t *lock)
{
    int rv;
#ifdef SINGLE_PROCESSOR
    return (pthread_mutex_lock(lock);
#else 
    while((rv=pthread_mutex_trylock(lock))==EBUSY);
    return rv;
#endif
}

int my_spinlock_limited(pthread_mutex_t *lock)
{
    int rv,count=MAX_SPIN;
#ifdef SINGLE_PROCESSOR
    return (pthread_mutex_lock(lock));
#else
    while((--count)&&(rv=pthread_mutex_trylock(lock))==EBUSY)
    {
        if((count==0)&&rv==EBUSY)
            return (pthread_mutex_lock(lock));
    }        
    return rv;
#endif
}
