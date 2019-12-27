#include "ch13.h"
#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include "err_exit.h"
#include <limits.h>
#include <unistd.h>
#define STACK_GROWS_DOWN
#undef STACK_GROWS_UP
void Hello(void)
{
    int array[512];
    printf("Hello,&array =%d\n",&array);
    Hello();
}
void *thread_stack_alloc(size_t size)
{
    int pagesize;
    void *men,*guard_addr,*stack_addr;
    if(size<PTHREAD_STACK_MIN)
    {
        fprintf(stderr,"thread_stack_alloc:SIZE less than PTHREAD_STACK_MIN\n");
        exit(1);
    }
    pagesize=sysconf(_SC_PAGESIZE);
    if(size%pagesize!=0)
        size=((size+pagesize)/pagesize)*pagesize;
    men=(void *)valloc(size+pagesize);
#ifdef STACK_GROWS_UP
    guard_addr=men+size;
    stack_addr=men;
#else
    guard_addr=men;
    stack_addr=men+pagesize;
#endif
    if(mprotect(guard_addr,pagesize,PROT_NONE))
        err_exit("mprotect");
    printf("stack area=(%d,%d)\n",stack_addr,stack_addr+size-1);
    printf("gaurded area=(%d,%d)\n",guard_addr,guard_addr+pagesize-1);
    return stack_addr;
}

int main()
{
    pthread_t thread;
    int rv;
    pthread_attr_t attr;
    size_t stacksize,guardsize;
    char *stackaddr;
    pthread_attr_init(&attr);
    stackaddr=(void *)thread_stack_alloc(PTHREAD_STACK_MIN);
    rv=pthread_attr_setstack(&attr,stackaddr,PTHREAD_STACK_MIN);
    check_error(rv,"pthread_setstack()");
    pthread_attr_getstack(&attr,(void *)&stackaddr,&stacksize);
    pthread_attr_getguardsize(&attr,&guardsize);
    printf("stack attrbutes:stackaddr=%d,stacksize=%d,guardsize=%d\n",stackaddr,stacksize,guardsize);
    rv=pthread_create(&thread,&attr,(void *(*)())Hello,(void *)NULL);
    check_error(rv,"pthread_create()");
    pthread_attr_destroy(&attr);
    check_error(rv,"pthread_attr_destroy");
    pthread_exit(NULL);
}
