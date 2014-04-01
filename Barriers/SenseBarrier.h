#ifndef _SenseBarrier_H_
#define _SenseBarrier_H_
#include "atomic_ops.h"
#include <pthread.h>
#include "barrier_def.h"


extern int num_thread;
extern int ROUND;

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

#ifdef _Barrier_TEST_
#define _Thread_data_
typedef struct thread_data{
	barrier_t *barrier;
	int thread_id;
	shared_obj_t *sc;
	bool_t threadSense;
        int num_cross;
}thread_data_t;
#endif

#ifndef _Thread_data_
#define _Thread_data_
typedef struct thread_data{
	barrier_t *barrier;
        barrier_def_t *barrier_def;
	struct timeval *start, *end;
	int thread_id;
	//shared_obj_t *sc;
	bool_t threadSense;
        int num_cross;
}thread_data_t;
#endif


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
		b->count = num_thread;
		b->sense = tdata->threadSense;
	}else{
		while(b->sense!=tdata->threadSense){}
	}
	tdata->threadSense = !tdata->threadSense;
}

#endif

