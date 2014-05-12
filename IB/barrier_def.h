#include <pthread.h>

#ifndef _barrier_def_h_
#define _barrier_def_h_

//default barrier for test
typedef struct barrier_def {
    pthread_cond_t complete;
    pthread_mutex_t mutex;
    int count;
    int crossing;
} barrier_def_t;

void barrier_init_def(barrier_def_t *b, int n)
{
    pthread_cond_init(&b->complete, NULL);
    pthread_mutex_init(&b->mutex, NULL);
    b->count = n;
    b->crossing = 0;
}

void barrier_cross_def(barrier_def_t *b)
{
    pthread_mutex_lock(&b->mutex);
    
    b->crossing++;
    
    if (b->crossing < b->count) {
        pthread_cond_wait(&b->complete, &b->mutex);
    } else {
        pthread_cond_broadcast(&b->complete);
        
        b->crossing = 0;
    }
    pthread_mutex_unlock(&b->mutex);
}

#endif


