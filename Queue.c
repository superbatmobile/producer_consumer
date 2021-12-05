#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>

Queue_t *initQ()
{
    Queue_t *q = calloc(1, sizeof(Queue_t));
    q->rear = 0;
    q->front = q->rear;
    pthread_mutex_init(&q->q_mutex, NULL);
    pthread_cond_init(&q->q_cv, NULL);
    q->count = 0;
    return q;
}

bool is_queue_empty(Queue_t *q)
{
    return q->count == 0 ? true : false;
}

bool is_queue_full(Queue_t *q)
{
    return q->count == Q_MAX_SIZE ? true : false;
}

bool enqueue(Queue_t *q, void *ptr)
{
    if (!q || !ptr){
        printf(" Queue or ptr is NULL\n");
        return false;
    }
    
    if (is_queue_full(q)){
        printf("queue is full\n");
        return false;
    }

    if (is_queue_empty(q)){
        q->elm[q->front] = ptr;
        q->count ++;
        return true;
    }
    if (q->rear == Q_MAX_SIZE - 1){
        q->rear = 0;
    }else{
        q->rear ++;
    }
    q->elm[q->rear] = ptr;
    q->count++;
    return true;
}

void *deque (Queue_t *q)
{
    void *elem = NULL;
    if (!q){
        printf("Queue doesn't exist\n");
        return NULL;
    }

    if (is_queue_empty(q)){
        printf("queue is empty\n");
        return NULL;
    }
    elem = q->elm[q->front];
    q->elm[q->front] = NULL;
    q->count--;
    if (q->front == q->rear){
        return elem;
    }
    if (q->front == Q_MAX_SIZE-1){
        q->front = 0;
    }else{
        q->front ++;
    }
    return elem;
}

void print_Queue(Queue_t *q)
{
    unsigned int i;
    //printf("q->front = %d, q->rear = %d, q->count = %d\n", q->front, q->rear, q->count);
    printf("printing elements in Queue\n");
    printf("----------\n");
    printf("[");
    for (i = 0; i < Q_MAX_SIZE; i++){
        printf("%d ", (int*)q->elm[i]);
    }
    printf("]");
    printf("\n");
    printf("----------\n");
}