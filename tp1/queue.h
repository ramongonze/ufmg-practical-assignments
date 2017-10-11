#include <stdio.h>
#include <stdlib.h>

typedef struct Cell{
	struct Cell *next;
	struct Cell *previous;
	int item;
} Cell;

typedef struct{
	Cell *first;
	Cell *last;
	int length;
} Queue;


Queue *doEmptyQueue();

void enqueue(Queue *queue, int item);

int dequeue(Queue *queue);

void printQueue(Queue *queue);

int checkEmptyQueue(Queue *queue);

void destroyQueue(Queue *queue);