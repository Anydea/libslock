#ifndef _SenseBarrier_H_
#define _SenseBarrier_H_
#include "atomic_ops.h"
#include "barrier_def.h"
#include <sched.h>

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
void barrier_init(barrier_t *b){
b->count = num_thread;
b->sense = false;
}


//set barrier_cross condition
void barrier_cross(barrier_t *b,thread_data_t* tdata){
	//printf("Crossing Start\n");
	tdata->threadSense = !b->sense;
	int temp,position;
	while(!stop){
		temp = b->count;
		if(temp == CAS_U32(&(b->count),temp,temp-1)){
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
		while(b->sense!=tdata->threadSense && !stop ){
			#ifdef MORE_THREADS
				sched_yield();
			#endif
		}
	}
	tdata->threadSense = !tdata->threadSense;
	//printf("Crossing End %d\n",tdata->num_cross);
}

#endif

