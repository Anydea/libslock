#ifdef _Barrier_TEST_
#define _Shared_data_
typedef struct shared_obj{
int counter;
int round;
}shared_obj_t;

void shared_init(shared_obj_t *s,int n){
	s->counter = n;
	s->round = ROUND;
}

int sqrt_my(int num){
	int i = 0;
	while(1){
		if(i*i == num)
			return i;
		if(i*i < num)
			i++;
		else
			return --i;
	}
}
#endif


int radix;
#ifdef USE_SenseBarrier
	#include "SenseBarrier.h"
#elif defined(USE_TreeBarrier)
	int num_backup;
	#include "TreeBarrier.h"
#elif defined(USE_StaticTreeBarrier)
	#include "StaticTreeBarrier.h"
#endif


#ifdef USE_SenseBarrier
typedef barrier_t BARRIER_t;
#elif defined(USE_TreeBarrier)
typedef TreeBarrier_t  BARRIER_t;
#elif defined(USE_StaticTreeBarrier)
typedef StaticTreeBarrier_t BARRIER_t;
#endif


static inline void InitBarrier(BARRIER_t* barrier){
#ifdef USE_SenseBarrier
	barrier_init(barrier);
#elif defined(USE_TreeBarrier)
	num_backup = num_thread;
	TreeBarrier_init(barrier);
#elif defined(USE_StaticTreeBarrier)
	StaticTreeBarrier_init(barrier);
#endif
}


static inline void CrossBarrier(thread_data_t* my_data){
#ifdef USE_SenseBarrier
	barrier_cross(my_data->barrier,my_data);
#elif defined(USE_TreeBarrier)
	TreeBarrier_cross(my_data->barrier,my_data);
#elif defined(USE_StaticTreeBarrier)
	StaticTreeBarrier_cross(my_data->barrier,my_data);
#endif
}


static inline void freeBarrier(){
#ifdef USE_SenseBarrier
#elif defined(USE_TreeBarrier)
	TreeBarrier_destroy();
#elif defined(USE_StaticTreeBarrier)
	StaticTreeBarrier_destroy();
#endif
}

