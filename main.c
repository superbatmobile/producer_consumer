#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include "Queue.h"
#include "Queue.c"

static int new_int(){
    static int a = 0;
    a++;
    return a;
}

Queue_t *Q;

static const char *prod1 = "P1";
static const char *prod2 = "P2";
static const char *cons1 = "C1";
static const char *cons2 = "C2";

static void *prod_fn(void *arg)
{
    print_Queue(Q);
    int i;
    char *th_name = (char *)arg;
    printf("Producer %s waiting to lock the Queue\n", th_name);
    pthread_mutex_lock(&Q->q_mutex);
    printf("Producer %s locks the Queue\n", th_name);
    while (is_queue_full(Q)){
        printf("Producer %s blocks itself, Q is already full\n", th_name);
        pthread_cond_wait(&Q->q_cv, &Q->q_mutex);
        printf("Producer %s wakes up, check the Queue status again\n", th_name);
    }
    assert(is_queue_empty(Q));
    while(!is_queue_full(Q)){
        i = new_int();
        printf("Producer %s produces new interger %d into Queue\n", th_name, i);
        enqueue(Q, (void *) i);
        printf("Producer %s pushed integer %d in Queue, Queue_size = %d\n",th_name,i,Q->count);
    }
    printf("Producer %s complete produce element in Queue , release lock and signal", th_name);
    printf("Producer %s job done and exited\n\n\n",th_name);
    print_Queue(Q);
    pthread_cond_broadcast(&Q->q_cv);
    pthread_mutex_unlock(&Q->q_mutex);
    return NULL;
}

static void *cons_fn(void *arg)
{
    print_Queue(Q);
    int i;
    char *th_name = (char *)arg;
    printf("Consumer %s is waiting to lock the queue\n", th_name);
    pthread_mutex_lock(&Q->q_mutex);
    printf("Consumer %s locks the Queue\n", th_name);
    while (is_queue_empty(Q)){
        printf("Consumer %s blocks itself, Q is already empty\n", th_name);
        pthread_cond_wait(&Q->q_cv, &Q->q_mutex);
        printf("Consumer %s wakes up, checking the Queue status again\n", th_name);
    }
    
    assert(is_queue_full(Q));
    while(!is_queue_empty(Q)){
        i = (int)deque(Q);
        printf("Consumer %s consumes an integer %d, Queue size = %d\n", th_name,i,Q->count);
    }
    printf("Consumer %s Drains the entire Queue, sending signal to Blocking Threads\n",th_name);
    pthread_cond_broadcast(&Q->q_cv);
    printf("Consumer %s is releasing lock\n", th_name);
    pthread_mutex_unlock(&Q->q_mutex);
    printf("Consumer %s finished and existed\n", th_name);
    print_Queue(Q);
    return NULL;
}

int main(){
    Q = initQ();
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    pthread_t prod_th1, prod_th2;
    pthread_t cons_th1, cons_th2;
    
    pthread_create( &prod_th1, &attr, prod_fn, (void *)prod1);
    pthread_create( &prod_th2, &attr, prod_fn, (void *)prod2);
    pthread_create( &cons_th1, &attr, cons_fn, (void *)cons1);
    pthread_create( &cons_th2, &attr, cons_fn, (void *)cons2);
    
    pthread_join(prod_th1, 0);
    pthread_join(prod_th2, 0);
    pthread_join(cons_th1, 0);
    pthread_join(cons_th2, 0);
    printf("Prgoram finished\n");
    pthread_exit(0);
    return 0;
}