#ifndef __QUEUE__
#define __QUEUE__

#include <stdbool.h>
#include <pthread.h>

#define Q_MAX_SIZE 20

typedef struct Queue
{
    void *elm[Q_MAX_SIZE];
    unsigned int front;
    unsigned int rear;
    unsigned int count;
    pthread_mutex_t q_mutex;
    pthread_cond_t q_cv;
}Queue_t;

Queue_t* initQ(void);

bool is_queue_empty(Queue_t *q);

bool is_queue_full(Queue_t *q);

bool enqueue(Queue_t *q, void *ptr);

void *deque(Queue_t *q);

void print_Queue(Queue_t *q);

#define Qcount(q) q->count
#endif