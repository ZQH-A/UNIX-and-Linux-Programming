#include <stdio.h>
#include <pthread.h>
#include "ch13.h"
#include <unistd.h>
int count;
int i=-1;
volatile int val=0;

void *thread_routine(void *arg)
{
    int state=PTHREAD_CANCEL_DISABLE;
    int oldstate,j;
    for(count=1;;count++){
        for(j=0;j<1000;j++)
            val=(val*j)/count;
        if(count%1024==0){
            i=1;
            pthread_testcancel();
        }else if((count-100)%2048==0){
            pthread_setcancelstate(state,&oldstate);
            for(j=0;j<1000000;j++)
                val=(val*j)/count;
            pthread_setcancelstate(oldstate,&state);
            i=0;
            pthread_testcancel();
        }
    }
}

int main()
{
    pthread_t tid;
    int rv;
    void *result;
    rv=pthread_create(&tid,NULL,thread_routine,NULL);
    check_error(rv,"Create thread");
    sleep(3);
    rv=pthread_cancel(tid);
    check_error(rv,"cancel thread");
    rv=pthread_join(tid,&result);
    if(result==PTHREAD_CANCELED)
        printf("thread %lx was canceled at itertion %d,i=%d\n",tid,count,i);
    else
        printf("thread %lx was not canceled.count=%d,i=%d\n",tid,count,i);
    return 0;
}
