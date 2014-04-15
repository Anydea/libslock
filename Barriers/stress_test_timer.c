
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include <signal.h>
#include "barrier_def.h"



volatile int stop = 0;

void trigger(int sig){
	stop =1;
}

int num_thread;
int ROUND;
float period;
int total_cross=0;

#define USE_SenseBarrier

//#define USE_TreeBarrier


//#define USE_StaticTreeBarrier

#include "barrier_if.h"
extern char* optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;




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

signal(SIGALRM,trigger);
alarm(period);

int round=0;
barrier_cross_def(my_data->barrier_def);


//gettimeofday(&(my_data->start[round]), NULL);
while(!stop){
	//for(round = 0; round < ROUND ;round++){
		/*if(round%num_thread == task_id){
			(my_data->sc->counter)++;
		}  */
	
		//gettimeofday(&(my_data->start[round]), NULL);
        	//printf("start %ld\n",(my_data->start[round]).tv_usec);
	
		#ifdef USE_SenseBarrier
		barrier_cross(my_data->barrier,my_data);
        	#elif defined(USE_TreeBarrier)
		TreeBarrier_cross(my_data->barrier,my_data);
		#elif defined(USE_StaticTreeBarrier)
		StaticTreeBarrier_cross(my_data->barrier,my_data);
		#endif
		
		my_data->num_cross++;
		//gettimeofday(&(my_data->end[round]), NULL);
		//printf("end %ld\n",(my_data->end[round]).tv_usec);
        
	
		/*if((my_data->sc->counter) != round){
		printf("Error\n");
		}
		barrier_cross(my_data->barrier,my_data); */
	
	//}
}

int temp = 0;
while(1){
	temp = total_cross;
	if(temp == CAS_U64(&total_cross,temp,temp+my_data->num_cross)){
		break;
	}else{
		continue;
	}
}
//gettimeofday(&(my_data->end[round-1]), NULL);
//printf("thread %0d over\n",task_id);
pthread_exit(NULL);
}

int main(int argc, char*argv[])
{
int ch;
opterr = 0;

//default setting
num_thread 	= 	10;
ROUND		=	10;
radix		=	2;
period		=	1;
while((ch = getopt(argc,argv,"n:r:f::ht::"))!=-1){
	switch(ch){
		case 'n': 
			num_thread = atoi(optarg);
			break;
		case 'r':
			ROUND = atoi(optarg);
			break;
		case 'f':
			radix = atoi(optarg);
			break;
		case 't':
			period = atoi(optarg);
			break;
		case 'h' :
			printf("-n NUM_THREAD\n-r NUM_ROUND\n-f Tree Radix\n");
			return -1;
		default  :
			printf("-n NUM_THREAD\n-r NUM_ROUND\n-f Tree Radix\n");
			return -1;
	}
}


#ifdef USE_SenseBarrier
//printf("SenseBarrier\n");
//printf("Setting:\nNUM_THREAD: %d\nNUM_ROUND: %d\n",num_thread,ROUND);
printf("%d %d ",num_thread,ROUND);
#elif defined(USE_TreeBarrier)
//printf("TreeBarrier\n");
//printf("Setting:\nNUM_THREAD: %d\nNUM_ROUND: %d\nTree Radix: %d\n",num_thread,ROUND,radix);
printf("%d %d %d ",num_thread,ROUND,radix);
#elif defined(USE_StaticTreeBarrier)
//printf("StaticTreeBarrier\n");
//printf("Setting:\nNUM_THREAD: %d\nNUM_ROUND: %d\nTree Radix: %d\n",num_thread,ROUND,radix);
printf("%d %d %d ",num_thread,ROUND,radix);
#endif

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
#ifdef USE_SenseBarrier

char bname[] = "SenseBarrier";
barrier_t barrier;
barrier_init(&barrier);
#elif defined(USE_TreeBarrier)

char bname[] = "TreeBarrier";
//radix = (int)(sqrt_my((int)num_thread))+1;
num_backup = num_thread;
TreeBarrier_t barrier;
TreeBarrier_init(&barrier);
#elif defined(USE_StaticTreeBarrier)
char bname[] = "StaticTreeBarrier";
//radix = (int)(2*sqrt_my((int)num_thread));
StaticTreeBarrier_t barrier;
StaticTreeBarrier_init(&barrier);
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
	data[i].barrier_def = &barrier_def;
        data[i].barrier = &barrier;
        data[i].start = start[i];
	data[i].end = end[i];
	data[i].num_cross =0;
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
/*	if(i == 0){
		cross_start = start[i][0].tv_sec*1000000+start[i][0].tv_usec;
		cross_end  = end[i][ROUND-1].tv_sec*1000000+end[i][ROUND-1].tv_usec;
	}else{
	cross_start=min(start[i][0].tv_sec*1000000+start[i][0].tv_usec,cross_start);
	//printf("%ld %ld\n",start[i][0].tv_sec,start[i][0].tv_usec);
	cross_end  = max(end[i][ROUND-1].tv_sec*1000000+end[i][ROUND-1].tv_usec,cross_end);
        //printf("%ld %ld\n",start[i][ROUND-1].tv_sec,start[i][ROUND-1].tv_usec);
	}
        //printf("%ld %ld\n",cross_start,cross_end);
*/    }
//double throughput = (double)ROUND*num_thread/(cross_end-cross_start) * 1000;



//printf("The Thoughput of %s is:%f threads/ms \n",bname, throughput); 
printf("%f\n",total_cross/(period*num_thread)); 
free(threads);
free(data);
#ifdef USE_SenseBarrier
#elif defined(USE_TreeBarrier)
TreeBarrier_destroy();
#elif defined(USE_StaticTreeBarrier)
StaticTreeBarrier_destroy();
#endif
return 0;
}

