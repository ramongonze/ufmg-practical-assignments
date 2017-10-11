#include <limits.h>
#include "queue.h"

#define MAX_VERTEX 1000
#define INFINITE LONG_MAX
#define WHITE -1
#define BLACK -2
#define NONE -3
#define ADDITIONAL_VERTEX 2 //A super origin and a super sink

typedef struct Graph{
	long **matrix;
	int *predecessor;
	int *color;
	int V;
	int E;
	int F;
	int C;
}TGraph;

TGraph* createGraph();

void readGraph(TGraph *G);

long findSmallestEdge(TGraph *G);

void destroyGraph(TGraph *G);

int BFS(TGraph *G);