
#ifndef _TreeBarrier_h_
#define _TreeBarrier_h_

#include "atomic_ops.h"
#include <sched.h>


typedef enum bool{
	false,
	true,
}bool_t;

typedef struct Node{
	int  count;
	int children;
	bool_t sense;
	struct Node * parent;
}Node_t;

Node_t ** Leaf_list;
//Node_t ** Inner_list;
//int * shared_counter;
int leaves = 0;
int total_inner = 0;	


typedef struct TreeBarrier{
	int radix;
	//int leaves_num;
	Node_t* root;
	//Node_t* Leaf_list;
	int depth;
	//bool_t sense;
}TreeBarrier_t;


#ifdef _Barrier_TEST_
#define _Thread_data_
typedef struct thread_data{
	TreeBarrier_t *barrier;
	int thread_id;
	shared_obj_t *sc;
	bool_t threadSense;
        int num_cross;
}thread_data_t;
#endif


#ifndef _Thread_data_
#define _Thread_data_
typedef struct thread_data{
	TreeBarrier_t *barrier;
        barrier_def_t *barrier_def;
	struct timeval *start, *end;
	int thread_id;
	//shared_obj_t *sc;
	bool_t threadSense;
        int num_cross;
}thread_data_t;
#endif

void node_init(Node_t *node){
	//printf("Node Init.\n");
	node->count = radix;
	node->children=0;
	node->sense = false;
	node->parent = NULL;
}

void node_parent(Node_t *node, Node_t * parent){
	node->parent = parent;
	parent->children++;
}

void node_update(Node_t *node, int is_leaf, int* remain){
	if(is_leaf){
		node->children = (*remain);
		node->count = node->children;
	}else{
		node->count = node->children;
	}
}

void node_cross(Node_t * node, thread_data_t* tdata){
	tdata->threadSense = !node->sense;
	bool_t mysense = tdata->threadSense;
	//printf("I'm  %d , having %d child(dren)\n",tdata->thread_id,node->children);
	int temp,position;

/*
	while(!stop){
		temp = node->count;
		//printf("temp: %d\n",temp);
		if(temp == CAS_U32(&(node->count),temp,temp-1)){
			position = temp;
			break;
		}
		else{
			continue;
		}
	}
*/	position = FAD_U32(&(node->count));
	//printf("Id: %d  position: %d .\n",tdata->thread_id, position);
	if(position == 1){
		if(node->parent != NULL){
			//printf("I'm not parent\n");
			node_cross(node->parent, tdata);
		}
		node->count = node->children;
		node->sense = mysense;
	}else{
		while(node->sense != mysense && !stop) {
			#ifdef MORE_THREADS
				sched_yield();
			#endif
		}
	}
	tdata->threadSense = !mysense;
}



void build(Node_t* parent, int depth){
	//int queue = 1;
	int *remaining;
	remaining = &num_backup;
	if(depth == 0){
		//printf("remaining nodes %d  %d\n",*remaining,num_backup);
		Leaf_list[leaves++] = parent;
		if((*remaining)>radix){
			(*remaining) = (*remaining)-radix;
			node_update(parent,1,&radix);
		}else{
			node_update(parent,1,remaining);
			(*remaining) =0;
		}
	}else{

		int i;
		for(i = 0; i< radix &&(*remaining)>0; i++){
			//printf("remaining nodes %d  %d\n",*remaining,num_backup);
			Node_t * child = (Node_t *)malloc(sizeof(Node_t));
			node_init(child);
			node_parent(child,parent);
			build(child,depth-1);
			node_update(parent,0,remaining);
		}
/*

		while(depth){
			int i;
			for(i=0;i<radix;i++){
				Node_t * child = (Node_t *)malloc(sizeof(Node_t));
				node_init(child);
				node_parent(child,parent);
				Inner_list[queue++] = child; 

		} */
	}
}


void TreeBarrier_init(TreeBarrier_t* barrier){
	//printf("TreeBarrier Init.\n");
	Leaf_list = (Node_t **)malloc((num_thread+radix-1)/radix*sizeof(Node_t));
	barrier->depth = 0;
	int leaf_nodes = (num_thread+radix-1)/radix;
	//int n = leaf_nodes+radix-1;
	while(leaf_nodes>1){
		total_inner += leaf_nodes;
		leaf_nodes = (leaf_nodes+radix-1)/radix;
		barrier->depth++;
	}
	//Inner_list = (Node_t **)malloc((++total_inner)*sizeof(Node_t));
	//if(radix^(barrier->depth)<num_thread){
	//	barrier->depth++;
	//}

	//printf("Depth: %d\n", barrier->depth);
	//shared_counter = (int*)malloc(depth*sizeof(int));
	barrier->radix = radix;
	//barrier->leaves_num = leaves;
	//barrier->Leaf_list = Leaf_list;
	barrier->root = (Node_t *)malloc(sizeof(Node_t));
	//total_inner++;
	//Inner_list[0]=barrier->root;
	node_init(barrier->root);
	build(barrier->root,barrier->depth);
}


void TreeBarrier_cross(TreeBarrier_t* b,thread_data_t* tdata){
	int id = tdata->thread_id;
	//printf("id/radix: %d\n",id/radix);
	Node_t * my_leaf = Leaf_list[id/radix];
	node_cross(my_leaf,tdata);
}


void destroy_recur(Node_t *node){
	if(node->parent != NULL){
		node->parent->count--;
		if(node->parent->count == 0){
			destroy_recur(node->parent);
		}
		free(node);
		//printf("free\n");
	}else{
		free(node);
		//printf("free\n");
	}
}
void TreeBarrier_destroy(){
	int i;
	for(i = 0; i<(num_thread+radix-1)/radix;i++){
		//printf("F\n");
		destroy_recur(Leaf_list[i]);
	}
	free(Leaf_list);


}

#endif
