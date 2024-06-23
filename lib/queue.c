#include "queue.h"

Queue* createQueue(uint32_t capacity)
{
    Queue* queue    = (Queue*)malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->rear = 0;
    queue->array               = (QueueNode*)malloc(queue->capacity * sizeof(QueueNode));
    return queue;
}

int isEmpty(Queue* queue)
{
    return queue->front == queue->rear;
}

void enqueue(Queue* queue, uint32_t node_index, size_t distance)
{
    queue->array[queue->rear].node_index = node_index;
    queue->array[queue->rear].distance   = distance;
    queue->rear++;
}

QueueNode dequeue(Queue* queue)
{
    return queue->array[queue->front++];
}

void freeQueue(Queue* queue)
{
    if (queue != NULL) {
        free(queue->array);
        free(queue);
    }
}
