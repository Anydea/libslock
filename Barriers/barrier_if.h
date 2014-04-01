extern int ROUND;
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
#endif

#ifdef USE_SenseBarrier
#include "SenseBarrier.h"
#elif defined(USE_TreeBarrier)
int radix = 5;
#include "TreeBarrier.h"
#endif



/*
#ifdef USE_SenseBarrier
typedef barrier_t barrier_t;
#endif

*/
