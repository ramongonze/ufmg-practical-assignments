#include "queue.h"

Queue *doEmptyQueue(){
	Queue *queue = (Queue*) malloc(sizeof(Queue));
	if(queue == NULL){
		printf("Error Creating!\n");
		exit(-1);
	}

	queue->length = 0;
	queue->first = queue->last = NULL;

	return queue;
}

void enqueue(Queue *queue, int item){
	if(queue == NULL){
		printf("Error Enqueuing!\n");
		exit(-1);
	}

	Cell *newCell = (Cell*) malloc(sizeof(Cell));
	if(newCell == NULL){
		printf("Error Creating!\n");
		exit(-1);
	}	


	newCell->item = item;

	if(queue->length == 0){
		newCell->previous = newCell->next = NULL;
		queue->first = queue->last = newCell;
	}
	else{
		newCell->next = NULL;
		newCell->previous = queue->last;
		queue->last->next = newCell;
		queue->last = newCell;
	}
	queue->length++;
}

int dequeue(Queue *queue){
	int item;

	if(queue == NULL){
		printf("Error Dequeuing!\n");
		exit(-1);
	}

	if(queue->first == NULL){
		printf("Error: Empty queue!\n");
		exit(-1);
	}

	Cell *aux = queue->first;
	item = aux->item;
	
	if(queue->length > 1){
		queue->first = queue->first->next;
		queue->first->previous = NULL;
		queue->length--;
		free(aux);
		return item;
	}
	else if(queue->length == 1){
		queue->first = queue->last = NULL;
		queue->length--;
		free(aux);
		return item;
	}

	return -1;
}

void printQueue(Queue *queue){
	Cell *aux = NULL;
	aux = queue->first;

	while(aux != NULL){
		printf("|%d| ", aux->item);
		aux = aux->next;
	}
	printf("\n");
}

int checkEmptyQueue(Queue *queue){
	return (queue->first == NULL);
}

void destroyQueue(Queue *queue){
	Cell *aux = queue->first;

	while(aux != NULL){
		queue->first = queue->first->next;
		free(aux);
		aux = queue->first;
	}
	free(queue);
}