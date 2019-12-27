#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#define NUM_THREADS 4
#define VECL 4*10000000
double a[VECL],b[VECL],sum;
struct args{
    pthread_t tid;
    int l_sum;
    int index;    
}thrag[NUM_THREADS];
void *dotsum(void *arg)
{
    struct args *ap=(struct args*)arg;
    int i,start,end;
    double l_sum=0.0;
    start=ap->index*(VECL/NUM_THREADS);
    end=start+(VECL/NUM_THREADS);
    printf("thread %i do from index %d to %d\n",ap->index,start,end-1);
    for(i=start;i<end;i++)
        l_sum+=(a[i]*b[i]);
    ap->l_sum=l_sum;
    return;
}

int main()
{
    int i;
    assert(VECL%4==0);
    for(i=0;i<VECL;i++)
        a[i]=b[i]=1.0;
    for(i=1;i<NUM_THREADS;i++)
    {
        thrag[i].index=i;
        pthread_create(&thrag[i].tid,NULL,dotsum,(void *)&thrag[i]);        
    }
    thrag[0].index=0;
    dotsum((void *)&thrag[0]);
    for(i=1;i<NUM_THREADS;i++)
        pthread_join(thrag[i].tid,(void **)NULL);
    sum=0.0;
    for(i=0;i<NUM_THREADS;i++)
        sum+=thrag[i].l_sum;
    printf("sum =%lf\n",sum);
    pthread_exit(NULL);
        
}
