#ifndef _TDBarrier_h_
#define _TDBarrier_h_

#include "atomic_ops.h"

typedef enum bool{
	false,
	true,
}bool_t;


typedef struct TDBarrier{
	int count;

}TDBarrier_t;

void TDBarrier_init(TDBarrier_t * tdb, int n){
	tdb->count = n;

}

void TDBarrier_setActive(TDBarrier_t* tdb, bool_t active){
	int temp;
	if(active){
		while(1){
			temp = tdb->count;
			if(temp == CAS_U64(&tdb->count,temp,temp-1))
				break;
			else
				continue;
		}
	}else{
		while(1){
			temp = tdb->count;
			if(temp == CAS_U64(&tdb->count,temp,temp+1))
				break;
			else
				continue;
		}
	}
}

bool_t TDBarrier_isTerminated(TDBarrier_t * tdb){
	return tdb->count == 0;
}

