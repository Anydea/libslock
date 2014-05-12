#ifndef _StaticTreeBarrier_h_
#define _StaticTreeBarrier_h_


#include "atomic_ops.h"
#include <math.h>



typedef enum bool{
	false,
	true,
}bool_t;

bool_t global_sense;

typedef struct Node{
	int children;
	int ChildCount;
	bool_t sense;
	struct Node * parent;
}Node_t;


Node_t ** Node_list;

int nodes = 0;

typedef struct StaticTreeBarrier{
	int radix;
	int leaves_num;
	Node_t*root;
	int depth;
}StaticTreeBarrier_t;

#ifdef _Barrier_TEST_
#define _Thread_data_
typedef struct thread_data{
	StaticTreeBarrier_t *barrier;
	int thread_id;
	shared_obj_t *sc;
	bool_t threadSense;
        int num_cross;
}thread_data_t;
#endif


#ifndef _Thread_data_
#define _Thread_data_
typedef struct thread_data{
	StaticTreeBarrier_t *barrier;
        barrier_def_t *barrier_def;
	struct timeval *start, *end;
	int thread_id;
	//shared_obj_t *sc;
	bool_t threadSense;
        int num_cross;
}thread_data_t;
#endif

void node_init(Node_t * node, Node_t * myParent, int count){
	node->children = count;
	node->ChildCount = node->children;
	node->parent = myParent;
}

void node_update(Node_t * node){
	node->ChildCount = node->children;
}

void node_cross(Node_t* node, thread_data_t* tdata){
	tdata->threadSense = !global_sense;
	bool_t mySense = tdata->threadSense;
	//printf("I'm  %d , having %d child(dren)\n",tdata->thread_id,node->children);
	while(node->ChildCount >0 && !stop){
	/*	if(tdata->thread_id == 1){		
		printf("I'm waiting %d\n",tdata->thread_id);
		} */
	}
	node->ChildCount = node->children;
	if(node->parent != NULL){
		int temp;
		while(!stop){
			temp=node->parent->ChildCount;
			if(temp == CAS_U64(&node->parent->ChildCount,temp,temp-1)){
				//printf("I'm  %d , changing parent %d child(dren)\n",tdata->thread_id,node->parent->ChildCount);
				break;
			}else{
				continue;
			}
		}
		while(global_sense != mySense && !stop){
			#ifdef MORE_THREADS
				pthread_yield();
			#endif
		}
	}else{
		//printf("I'm  %d , PARENT\n",tdata->thread_id);
		global_sense = !global_sense;
	}
	tdata->threadSense = !mySense;
}

void build(Node_t* parent){
	int queue =0;
	//int deep=0;
	
/*	if(depth == 0){
		Node_t * leaf = (Node_t *)malloc(sizeof(Node_t));
		node_init(leaf,parent,0);
		Node_list[nodes++] = leaf;
	}else{
*/	/*	Node_t * branch = (Node_t *)malloc(sizeof(Node_t));
		node_init(branch,parent,0);
		Node_list[nodes++] = branch;
		int i;
		for(i = 0; i< radix && nodes<num_thread; i++){
			branch->children++;
			build(branch,depth-1);
		}
		node_update(branch);
	*/	

	Node_t * peak = (Node_t *)malloc(sizeof(Node_t));
	node_init(peak,parent,0);
	Node_list[nodes++] = peak;
	//deep++;
	
	while(nodes<num_thread){
		
		int i;//max_nodes=0;
	/*	for(i=0;i<=deep;i++){
			max_nodes=max_nodes+pow(radix,i);
		}
	*/	if(Node_list[queue]->ChildCount==0){
			for(i=0;i<radix && nodes<num_thread; i++){
				Node_t * branch =(Node_t *)malloc(sizeof(Node_t));
				node_init(branch,Node_list[queue],0);
				Node_list[nodes++] = branch;
				Node_list[queue]->children++;
			}
			node_update(Node_list[queue]);
			queue++;
		}
	/*	if(nodes>=max_nodes){
			deep++;
		}
	*/
	}
}

void StaticTreeBarrier_init(StaticTreeBarrier_t * barrier){
	Node_list = (Node_t **)malloc(num_thread*sizeof(Node_t));
	nodes = 0;
	barrier->depth = 0;
	int n;
	while(1){
	//	barrier->depth++;
	//	n = n/radix;
	
		int total =0;
		for(n=0;n<=barrier->depth;n++){
			total=total+pow(radix,n);
		}
		if(total<num_thread){
			barrier->depth++;
		}else{
			break;
		}

	}
	printf("Depth: %d\n", barrier->depth);
	barrier->radix = radix;
	//barrier->root = NULL;
	//node_init(barrier->root);
	build(NULL);
	global_sense = false;
}


void StaticTreeBarrier_cross(StaticTreeBarrier_t * b, thread_data_t* tdata){
	int id = tdata->thread_id;
	//printf("thread %d is at barrier\n", id);
	//printf("\n\n\n\n");
	Node_t * my_node = Node_list[id];
	node_cross(my_node,tdata);
}


void StaticTreeBarrier_destroy(){
	int i;
	for(i = 0; i<num_thread;i++){
		free(Node_list[i]);
	}
	free(Node_list);
}
#endif
