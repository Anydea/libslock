
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


int num_thread = 5;
int ROUND = 20;

#define _Barrier_TEST_

//#define USE_SenseBarrier

#define USE_TreeBarrier

#include "barrier_if.h"







//threads test
void *test_thread(void* data){

thread_data_t * my_data = (thread_data_t *) data;
int task_id = my_data->thread_id;



int round;


for(round = 0; round < ROUND ;round++){
	if(round%num_thread == task_id){
		(my_data->sc->counter)++;
	} 
	
	#ifdef USE_SenseBarrier
	barrier_cross(my_data->barrier,my_data);
        #elif defined(USE_TreeBarrier)
	TreeBarrier_cross(my_data->barrier,my_data);
	#endif
	
	
	if((my_data->sc->counter) != round){
	printf("Error\n");
	}
	#ifdef USE_SenseBarrier
	barrier_cross(my_data->barrier,my_data);
        #elif defined(USE_TreeBarrier)
	TreeBarrier_cross(my_data->barrier,my_data);
	#endif
	
}
printf("thread %0d over\n",task_id);
pthread_exit(NULL);
}

int main()
{
pthread_t *threads;
pthread_attr_t attr;
thread_data_t *data;
int i;


shared_obj_t sc;
shared_init(&sc,-1);

//global barrier
#ifdef USE_SenseBarrier
barrier_t barrier;
barrier_init(&barrier,num_thread);
#elif defined(USE_TreeBarrier)
TreeBarrier_t barrier;
TreeBarrier_init(&barrier,num_thread);
#endif

//initialize the data which will be passed to the threads
    if ((data = (thread_data_t *)malloc(num_thread * sizeof(thread_data_t))) == NULL) {
        perror("malloc");
        exit(1);
    }

    if ((threads = (pthread_t *)malloc(num_thread * sizeof(pthread_t))) == NULL) {
        perror("malloc");
        exit(1);
    }

pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    //set the data for each thread and create the threads
for ( i = 0; i < num_thread; i++) {
        data[i].thread_id = i;
        data[i].barrier = &barrier;
	data[i].sc = &sc;
        if (pthread_create(&threads[i], &attr, test_thread, (void *)(&data[i])) != 0) {
            fprintf(stderr, "Error creating thread\n");
            exit(1);
        }
    }

pthread_attr_destroy(&attr);

/* Wait for thread completion */
for ( i = 0; i < num_thread; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error waiting for thread completion\n");
            exit(1);
        }
    }
free(threads);
free(data);
return 0;
}

