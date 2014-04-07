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



