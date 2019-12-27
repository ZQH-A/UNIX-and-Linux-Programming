#include <stdio.h>
#include <pthread.h>
#include "ch13.h"

pthread_rwlock_t rw=PTHREAD_RWLOCK_INITIALIZER;
struct list{
    struct list *next;
    struct list *prev;
    int id;
};
struct list *list_head=NULL;
void list_add(struct list *add)
{
    struct list *curr,*prev;
    int rv;
    rv=pthread_rwlock_wrlock(&rw);
    check_error(rv,"add:write lock");
    if(list_head==(struct list *)NULL)
    {
        list_head=add;
        add->prev=add->next=NULL;
        
    }else{
        for(curr=list_head;curr!=NULL;curr->curr->next){
            if(curr->id>add->id)
                break;
            prev=curr;
        }
        if(curr==NULL){
            add->next=NULL;
            add->prev=prev;
            prev->next=add;
        }else{
            add->next=curr;
            add->prev=prev;
            prev->next=add;
            curr->prev=add;
        }
            
    }
    rv=pthread_rwlock_unlock(&rw);
    check_error(rv,"add:write unlock");
}

struct list* list_lookup(int lookup_id)
{
    struct list *curr;
    int rv;
    rv=pthread_rwlock_rdlock(&rw);
    check_error(rv,"lookup:read lock");
    for(curr=list_head;curr!=NULL;curr=curr->next)
    {
        if(curr->id>=lookup_id)
            break;
        
    }
    if(curr!=NULL&&curr->id!=lookup_id)
        curr=(struct list*)NULL;
    rv=pthread_rwlock_unlock(&rw);
    check_error(rv,"add:write unlock");
    return (curr);
}
