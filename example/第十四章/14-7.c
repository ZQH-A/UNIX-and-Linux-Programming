#include "ch13.h"
#include "14-6.c"
#include "13-13.c"
#include <assert.h>
#define OUTPUTSCHEDPARAM(policy,priority)  printf(           \
                        "Thread %lx:Policy=%s,priority=%d\n",\
                        pthread_self(),                      \
                        (policy==SCHED_FIFO?"FIFO"           \
                            :(policy==SCHED_RR?"RR"          \
                            :(policy==SCHED_OTHER?"OTHER"    \
                            :"unknown"))),priority)           
barrier_t barrier;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
void *t_routine(void *n)
{
    int rv,mypolicy,thread_no=(int)n;
    struct sched_param myparam;
    barrier_wait(&barrier);
    rv=pthread_mutex_lock(&mutex);
    check_error(rv,"mutex_lock");
    printf("Thread %d:",thread_no);
    check_error(rv,"getschedparam");
    rv=pthread_getschedparam(pthread_self(),&mypolicy,&myparam);
    check_error(rv,"getschedparam");
    
    OUTPUTSCHEDPARAM(mypolicy,myparam.sched_priority);
    rv=pthread_mutex_unlock(&mutex);
    check_error(rv,"mutex_unlock");
}

int main()
{
    int rv,priority,policy;
    struct sched_param param;
    struct sched_thread t_param;

    rv=barrier_init(&barrier,5);
    check_error(rv,"barrier_init");
    
    rv=pthread_getschedparam(pthread_self(),&policy,&param);
    check_error(rv,"main:getschedparam");
    OUTPUTSCHEDPARAM(policy,param.sched_priority);
    
    t_param.thread_func=t_routine;
    t_param.policy=SCHED_FIFO;
    t_param.num=1;
    t_param.priority=sched_get_priority_min(SCHED_FIFO);
    create_exlicit_sched_threads(&t_param);
    
    t_param.num=2;
    t_param.priority +=2;
    create_exlicit_sched_threads(&t_param);
    
    t_param.policy=SCHED_RR;
    t_param.num=3;
    t_param.priority=sched_get_priority_min(SCHED_RR);
    create_exlicit_sched_threads(&t_param);
    
    t_param.num=4;
    t_param.priority=sched_get_priority_max(SCHED_RR);
    create_exlicit_sched_threads(&t_param);
    
    if(policy==SCHED_RR)
    {
        rv=pthread_setschedprio(pthread_self(),t_param.priority+6);
        check_error(rv,"getschedparam");
    }else{
        param.sched_priority +=10;
        rv=pthread_setschedparam(pthread_self(),SCHED_RR,&param);
        check_error(rv,"getschedparam");        
    }
    t_routine(0);
    pthread_exit((void *)NULL);
}

                            
                                                            
