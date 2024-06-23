#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <stdlib.h>

typedef struct
{
    uint32_t node_index; // Индекс узла
    uint32_t distance;   // Расстояние от исходного узла
} QueueNode;

typedef struct
{
    uint32_t   front, rear;
    uint32_t   capacity;
    QueueNode* array;
} Queue;

// Создание очереди
Queue* createQueue(uint32_t capacity);

// Проверка, пуста ли очередь
int isEmpty(Queue* queue);

// Добавление элемента в очередь
void enqueue(Queue* queue, uint32_t node_index, size_t distance);

// Удаление элемента из очереди
QueueNode dequeue(Queue* queue);

// очистка памяти
void freeQueue(Queue* queue);

#endif // QUEUE_H
