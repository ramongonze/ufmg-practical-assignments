#include "graph.h"

TGraph* createGraph(){
	return (TGraph *) malloc(sizeof(TGraph));
}

void readGraph(TGraph *G){
	int i, j, v1, v2;
	long peso;

	scanf("%d %d %d %d", &(G->V), &(G->E), &(G->F), &(G->C));

	G->predecessor = (int *) malloc((G->V + ADDITIONAL_VERTEX)* sizeof(int));
	G->color = (int *) calloc((G->V + ADDITIONAL_VERTEX),sizeof(int));

	G->matrix = (long **) malloc((G->V + ADDITIONAL_VERTEX) * sizeof(long *));
	for(i = 0; i < (G->V + 2); i++){
		G->matrix[i] = (long *) malloc((G->V + ADDITIONAL_VERTEX) * sizeof(long));
	}

	for(i = 0; i < (G->V + ADDITIONAL_VERTEX); i++){
		for(j = 0; j < (G->V + ADDITIONAL_VERTEX); j++){
			G->matrix[i][j] = 0;
		}
	}

	for(i = 0; i < G->E; i++){
		scanf("%d %d %ld", &v1, &v2, &peso);
		G->matrix[v1][v2] = peso;
	}

	for(i = 0; i < G->F; i++){
		scanf("%d", &v1);
		G->matrix[G->V][v1] = INFINITE;
	}

	for(i = 0; i < G->C; i++){
		scanf("%d", &v1);
		G->matrix[v1][G->V + 1] = INFINITE;
	}
}

void destroyGraph(TGraph *G){
	int i;

	for(i = 0; i < (G->V + ADDITIONAL_VERTEX); i++){
		free(G->matrix[i]);
	}
	free(G->matrix);
	free(G->predecessor);
	free(G->color);
	free(G);
}

long findSmallestEdge(TGraph *G){
	int u, v;
	long smallest_edge;

	//Find the smallest edge in the path gerated by BFS execution
	v = G->predecessor[G->V + 1];
	smallest_edge = G->matrix[G->predecessor[v]][v];
	u = G->predecessor[v];
	while(u != G->V){
		u = G->predecessor[v];
		if(G->matrix[u][v] < smallest_edge){
			smallest_edge = G->matrix[u][v];
		}
		v = u;
	}
	//Decrease the value of the smallest edge in all the edges of the path
	v = G->V + 1;
	u = G->predecessor[v];
	while(u != G->V){
		u = G->predecessor[v];
		if(G->matrix[u][v] != INFINITE){
			G->matrix[u][v] -= smallest_edge;
		}
		G->matrix[v][u] = smallest_edge;
		v = u;
	}

	return smallest_edge;
}

int BFS(TGraph *G){
	int i, u, v, test;
	Queue *q;

	q = doEmptyQueue();

	for(i = 0; i < (G->V + ADDITIONAL_VERTEX); i++){
		G->predecessor[i] = NONE;
		G->color[i] = WHITE;
	}

	u = G->V;
	G->color[u] = BLACK;
	enqueue(q, u);

	while(checkEmptyQueue(q) == 0){
		u = dequeue(q);
		for(v = 0; v < (G->V + ADDITIONAL_VERTEX); v++){
			if(G->matrix[u][v] > 0 && G->color[v] == WHITE){
				G->color[v] = BLACK;
				G->predecessor[v] = u;
				enqueue(q, v);
			}
		}
	}

	destroyQueue(q);
	
	//Tests if there is a valid path between the sink and the origin
	test = 1;
	v = G->V + 1;
	while(u != G->V){
		u = G->predecessor[v];
		if(u == NONE){
			test = 0;
			break;
		}
		v = u;
	}

	return test;
}
