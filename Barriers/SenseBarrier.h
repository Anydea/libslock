#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include "atomic_ops.h"


void barrier_init(barrier_t *b, int NUM_THREAD);
void barrier_cross(barrier_t *b,thread_data_t* tdata);




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


/*
//shared obj
typedef struct shared_obj{
int counter;
int round;
}shared_obj_t;


void shared_init(shared_obj_t *s,int NUM_THREAD,int Round){
	s->counter = NUM_THREAD;
	s->round = Round;
}

//information sent to threads
typedef struct thread_data{
barrier_t *barrier;
int thread_id;
shared_obj_t *sc;
bool_t threadSense;
int NUM_THREAD;
}thread_data_t;
*/

//barrier initialization
void barrier_init(barrier_t *b, int NUM_THREAD){
b->count = NUM_THREAD;
b->sense = false;
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

