#include "functions.h"
#include <cstring>

#ifndef VERTEX
#include "graph.h"
#define VERTEX 1
#endif

#define MAX_DATA_NAME 100

int main(int argc, char *argv[]){
	char train[MAX_DATA_NAME], test[MAX_DATA_NAME];
	Graph G;

	strcpy(train, argv[1]);
	strcpy(test, argv[2]);

	G = readData(train);

	calculateMeans(G);

	

	return 0;
}