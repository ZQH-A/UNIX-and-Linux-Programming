#include "ch13.h"
#include <pthread.h>
extern struct job*create_job();
extern void job_enqueue(struct job*ptr);
typedef struct job_sync_data
{
    int job_count;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
}job_sync_data;
extern job_sync_data job_sync;
void master_thread(0
{
    struct job *new_job;
    int rv;
    for(;;)
    {
        if((new_job=create_job())==NULL)
            pthread_exit((void *)NULL);
        
        rv=pthread_mutex_lock(&job_sync.mtx);
        check_error(rv,"mutex lock");
        job_enqueue(new_job);
        job_sync.job_count++;
        
        rv=pthread_cond_signal(&job_sync.cond);
        check_error(rv,"cond_signal");
        rv=pthread_mutex_unlock(&job_sync.mtx);
        check_error(rv,"mtx_unlock");
    }
}
