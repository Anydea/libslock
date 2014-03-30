
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include "atomic_ops.h"
#include "barrier_def.h"


#define USE_SenseBarrier

#include "barrier_if.h"
extern char* optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;


int num_thread;
int ROUND;


double min(double new, double old){
	if(new < old){
		return new;
	}
	return old;
}

double max(double new, double old){
	if(new > old){
		return new;
	}
	return old;
}


typedef struct shared_obj{
int counter;
int round;
}shared_obj_t;


#ifndef _Thread_data_
#define _Thread_data_
//information sent to threads
typedef struct thread_data{
	barrier_t *barrier;
        barrier_def_t *barrier_def;
	struct timeval *start, *end;
	int thread_id;
	shared_obj_t *sc;
	bool_t threadSense;
        int num_cross;
}thread_data_t;
#endif



void shared_init(shared_obj_t *s,int n){
	s->counter = n;
	s->round = ROUND;
}

//threads test
void *test_thread(void* data){

thread_data_t * my_data = (thread_data_t *) data;
int task_id = my_data->thread_id;



int round;
barrier_cross_def(my_data->barrier_def);

for(round = 0; round < ROUND ;round++){
	/*if(round%num_thread == task_id){
		(my_data->sc->counter)++;
	}  */
	
	gettimeofday(&(my_data->start[round]), NULL);
        //printf("start %ld\n",(my_data->start[round]).tv_usec);
	barrier_cross(my_data->barrier,my_data);

	gettimeofday(&(my_data->end[round]), NULL);
	//printf("end %ld\n",(my_data->end[round]).tv_usec);
        
	
	/*if((my_data->sc->counter) != round){
	printf("Error\n");
	}
	barrier_cross(my_data->barrier,my_data); */
	
}
printf("thread %0d over\n",task_id);
pthread_exit(NULL);
}

int main(int argc, char*argv[])
{
int ch;
opterr = 0;


while((ch = getopt(argc,argv,"n:r:"))!=-1){
	switch(ch){
		case 'n': 
			num_thread = atoi(optarg);
			break;
		case 'r':
			ROUND = atoi(optarg);
			break;
		default :
			printf("-n NUM_THREAD\n-r NUM_ROUND\n");
			return 1;
			
	}
}

struct timeval start[num_thread][ROUND], end[num_thread][ROUND];

pthread_t *threads;
pthread_attr_t attr;
thread_data_t *data;
int i;


//shared_obj_t sc;
//shared_init(&sc,-1);


//global default barrier
barrier_def_t barrier_def;
barrier_init_def(&barrier_def,num_thread);


//global test barrier
barrier_t barrier;
barrier_init(&barrier,num_thread);

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
	data[i].barrier_def = &barrier_def;
        data[i].barrier = &barrier;
        data[i].start = start[i];
	data[i].end = end[i];
	//data[i].sc = &sc;
        if (pthread_create(&threads[i], &attr, test_thread, (void *)(&data[i])) != 0) {
            fprintf(stderr, "Error creating thread\n");
            exit(1);
        }
    }

pthread_attr_destroy(&attr);


long int cross_start;
long int cross_end;


/* Wait for thread completion */
for ( i = 0; i < num_thread; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error waiting for thread completion\n");
            exit(1);
        }
	if(i == 0){
		cross_start = start[i][0].tv_sec*1000000+start[i][0].tv_usec;
		cross_end  = end[i][ROUND-1].tv_sec*1000000+end[i][ROUND-1].tv_usec;
	}else{
	cross_start=min(start[i][0].tv_sec*1000000+start[i][0].tv_usec,cross_start);
	//printf("%ld %ld\n",start[i][0].tv_sec,start[i][0].tv_usec);
	cross_end  = max(end[i][ROUND-1].tv_sec*1000000+end[i][ROUND-1].tv_usec,cross_end);
        //printf("%ld %ld\n",start[i][ROUND-1].tv_sec,start[i][ROUND-1].tv_usec);
	}
        //printf("%ld %ld\n",cross_start,cross_end);
    }
double throughput = (double)ROUND*num_thread/(cross_end-cross_start) * 1000;

printf("The Thoughput of Sense Barrier is:%f threads/ms \n", throughput); 
free(threads);
free(data);
return 0;
}

