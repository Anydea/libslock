
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


#define NUM_THREAD 20

//bool type
typedef enum bool {
false,
true
}bool_t;

//sense barrier implementation
typedef struct barrier{
pthread_mutex_t count_mut;
int count;
bool_t sense;
bool_t threadSense[NUM_THREAD];
}barrier_t;


typedef struct shared_obj{
int counter;
}shared_obj_t;

//information sent to threads
typedef struct thread_data{
barrier_t *barrier;
int thread_id;
shared_obj_t *sc;

}thread_data_t;




//barrier initialization
void barrier_init(barrier_t *b, int n){
b->count = n;
b->sense = false;
pthread_mutex_init(&b->count_mut,NULL);
}

void shared_init(shared_obj_t *s,int n){
	s->counter = n;
}

//set barrier_cross condition
void barrier_cross(barrier_t *b,int n){
	b->threadSense[n] = !b->sense;
	pthread_mutex_lock(&b->count_mut);
		int position  = b->count--;
	pthread_mutex_unlock(&b->count_mut);
	if(position == 1){
		b->count = NUM_THREAD;
		b->sense = b->threadSense[n];
	}else{
		while(b->sense!=b->threadSense[n]){}
	}
	b->threadSense[n] = !b->threadSense[n];
}

//threads test
void *test_thread(void* data){

thread_data_t * my_data = (thread_data_t *) data;
int task_id = my_data->thread_id;

int round;
//printf("thread %02d is going to sleep\n",task_id);

for(round = 0; round < 3 ;round++){
//printf("thread %02d gets %02d\n",task_id, i);
	if(round%NUM_THREAD == task_id){
		(my_data->sc->counter)++;
		//printf("thread %02d gets %02d in round %d\n",task_id, sc,round);
	}
	//sc = my_data->sc;
	printf("thread %02d gets %02d in round %d\n",task_id, (my_data->sc->counter),round);
	barrier_cross(my_data->barrier,task_id);
	
	if((my_data->sc->counter) != round){
	printf("Error\n");
	}
	barrier_cross(my_data->barrier,task_id);
}
printf("thread %0d over\n",task_id);
pthread_exit(NULL);
}

int main()
{
pthread_t *threads;
pthread_attr_t attr;
thread_data_t *data;
int i;


shared_obj_t sc;
shared_init(&sc,-1);

//global barrier
barrier_t barrier;
barrier_init(&barrier,NUM_THREAD);

//initialize the data which will be passed to the threads
    if ((data = (thread_data_t *)malloc(NUM_THREAD * sizeof(thread_data_t))) == NULL) {
        perror("malloc");
        exit(1);
    }

    if ((threads = (pthread_t *)malloc(NUM_THREAD * sizeof(pthread_t))) == NULL) {
        perror("malloc");
        exit(1);
    }

pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    //set the data for each thread and create the threads
for ( i = 0; i < NUM_THREAD; i++) {
   
        data[i].thread_id = i;
        data[i].barrier = &barrier;
	data[i].sc = &sc;
        if (pthread_create(&threads[i], &attr, test_thread, (void *)(&data[i])) != 0) {
            fprintf(stderr, "Error creating thread\n");
            exit(1);
        }
    }

pthread_attr_destroy(&attr);

/* Wait for thread completion */
for ( i = 0; i < NUM_THREAD; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error waiting for thread completion\n");
            exit(1);
        }
    }
free(threads);
free(data);
return 0;
}

