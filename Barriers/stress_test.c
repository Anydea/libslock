
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include "atomic_ops.h"



extern char* optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;


//bool type
typedef enum bool {
false,
true
}bool_t;

//sense barrier implementation
typedef struct barrier{
int count;
bool_t sense;
}barrier_t;


typedef struct shared_obj{
int counter;
int round;
}shared_obj_t;

//information sent to threads
typedef struct thread_data{
barrier_t *barrier;
int thread_id;
shared_obj_t *sc;
bool_t threadSense;
int NUM_THREAD;
}thread_data_t;




//barrier initialization
void barrier_init(barrier_t *b, int n){
b->count = n;
b->sense = false;
}

void shared_init(shared_obj_t *s,int n,int r){
	s->counter = n;
	s->round = r;
}

//set barrier_cross condition
void barrier_cross(barrier_t *b,thread_data_t* tdata){
	tdata->threadSense = !b->sense;
	int temp,position;
	while(1){
		temp = b->count;
		if(temp == CAS_U64(&(b->count),temp,temp-1)){
			position = temp;
			break;
		}
		else{
			continue;
		}
	
	} 
	if(position == 1){
		b->count = tdata->NUM_THREAD;
		b->sense = tdata->threadSense;
	}else{
		while(b->sense!=tdata->threadSense){}
	}
	tdata->threadSense = !tdata->threadSense;
}

//threads test
void *test_thread(void* data){

thread_data_t * my_data = (thread_data_t *) data;
int task_id = my_data->thread_id;
int ROUND = my_data->sc->round;


int round;

for(round = 0; round < ROUND ;round++){
	if(round%my_data->NUM_THREAD == task_id){
		(my_data->sc->counter)++;
	}
	barrier_cross(my_data->barrier,my_data);
	
	if((my_data->sc->counter) != round){
	printf("Error\n");
	}
	barrier_cross(my_data->barrier,my_data);
}
printf("thread %0d over\n",task_id);
pthread_exit(NULL);
}

int main(int argc, char*argv[])
{
int ch,NUM_THREAD,test_round;
opterr = 0;
struct timeval start, end;
struct timespec timeout;

while((ch = getopt(argc,argv,"n:r:"))!=-1){
	switch(ch){
		case 'n': 
			NUM_THREAD = atoi(optarg);
			break;
		case 'r':
			test_round = atoi(optarg);
			break;
		default :
			printf("-n NUM_THREAD\n-r NUM_ROUND\n");
			return 1;
			
	}
}


pthread_t *threads;
pthread_attr_t attr;
thread_data_t *data;
int i;


shared_obj_t sc;
shared_init(&sc,-1,test_round);

//global barrier
barrier_t barrier;
barrier_init(&barrier,NUM_THREAD);

//initialize the data which will be passed to the threads
    if ((data = (thread_data_t *)malloc(NUM_THREAD * sizeof(thread_data_t))) == NULL) {
        perror("malloc");
        exit(1);
    }

    if ((threads = (pthread_t *)malloc(NUM_THREAD * sizeof(pthread_t))) == NULL) {
        perror("malloc");
        exit(1);
    }

pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    //set the data for each thread and create the threads
for ( i = 0; i < NUM_THREAD; i++) {
   	
	data[i].NUM_THREAD = NUM_THREAD;
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
for ( i = 0; i < NUM_THREAD; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error waiting for thread completion\n");
            exit(1);
        }
    }
free(threads);
free(data);
return 0;
}

