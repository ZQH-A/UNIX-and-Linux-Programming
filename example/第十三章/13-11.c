#include "ch13.h"
#include <pthread.h>
typedef struct job_sync_data
{
    int job_count;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
}job_sync_data;
job_sync_data job_sync={
    0,PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER};
struct jov{
    int job_id;
    void * work_ptr;
};
extern struct job*job_dequeue();
extern void procee_job(struct job *ptr);
void work_thread()
{
    struct job *curr_job;
    int rv;
    for(;;){
        rv=pthread_mutex_lock(&job_sync.mtx);
        check_error(rv,"mutex lock");
        
        while(job_sync.job_count==0)
            pthread_cond_wait(&job_sync.cond,&job_sync.mtx);
        curr_job=job_dequeue();
        if(curr_job!=NULL)
            job_sync.job_count--;
        rv=pthread_mutex_unlock(&job_sync.mtx);
        check_error(rv,"mtx_unlock failed");
        procee_job(curr_job);
    }
}
