#include "ch13.h"
#include <pthread.h>
#include <stdio.h>

pthread_key_t key1,key2;
void do_work(void)
{
    int num;
    char *vp;
    num=(int)pthread_getspecific(key1);
    vp=(char *)pthread_getspecific(key2);
    sprintf(vp,"\tThread %d is at work now,key2's value:%x",num,vp);    
}
void start_func(int thread_num)
{
    char *buf,*vp;
    pthread_setspecific(key1,(void*)thread_num);
    buf=(char *)malloc(100);
    pthread_setspecific(key2,(void *)buf);
    do_work();
    vp=(char *)pthread_getspecific(key2);
    printf("Thread %d's thread specific data is: \n %s \n",thread_num,vp);
    if(thread_num!=0)
        pthread_exit(NULL);
}
void destructor(void *value)
{
    free(value);
    printf("memory %x released by destructor\n",value);
}

int main(int argc,char *argv[])
{
    int rv;
    pthread_t tid;
    rv=pthread_key_create(&key1,NULL);
    check_error(rv,"Create key1");
    rv=pthread_key_create(&key2,destructor);
    check_error(rv,"Create key2");
    
    rv=pthread_create(&tid,NULL,(void*(*)())start_func,(void *)1);
    check_error(rv,"Thread create");
    
    start_func(0);
    rv=pthread_join(tid,NULL);
    check_error(rv,"Thread join");
    rv=pthread_key_delete(key1);
    check_error(rv,"key1 delete");
    pthread_exit(NULL);
}
