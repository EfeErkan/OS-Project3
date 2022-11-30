#ifndef CIRCULAR_QUEUE
#define CIRCULAR_QUEUE

// Definitions

struct circular_queue
{
    int front;
    int rear;
    int size;
    int *arr;
};

void init_queue(struct circular_queue *queue, int size);

void free_queue(struct circular_queue *queue);

int isFull(struct circular_queue *queue);

void enqueue(struct circular_queue *queue, int value);

int dequeue(struct circular_queue *queue);


// Implementation

void init_queue(struct circular_queue *queue, int size)
{
    queue->front = -1;
    queue->rear = -1;
    queue->size = size;
    queue->arr = malloc(sizeof(int) * size);
}

void free_queue(struct circular_queue *queue)
{
    free(queue->arr);
}

int isFull(struct circular_queue *queue)
{
    if ((queue->front == 0 && queue->rear == queue->size - 1) ||
            (queue->rear == (queue->front-1)%(queue->size - 1)))
    {
        return 1;
    }
    return 0;
}

void enqueue(struct circular_queue *queue, int value)
{
    if (queue->front == -1) /* Insert First Element */
    {
        queue->front = queue->rear = 0;
        queue->arr[queue->rear] = value;
    }
    else if (queue->rear == queue->size - 1 && queue->front != 0)
    {
        queue->rear = 0;
        queue->arr[queue->rear] = value;
    }
    else
    {
        queue->rear++;
        queue->arr[queue->rear] = value;
    }
}

int dequeue(struct circular_queue *queue)
{
    int data = queue->arr[queue->front];
    queue->arr[queue->front] = -1;
    if (queue->front == queue->rear)
    {
        queue->front = -1;
        queue->rear = -1;
    }
    else if (queue->front == queue->size - 1)
        queue->front = 0;
    else
        queue->front++;
  
    return data;
}

#endif