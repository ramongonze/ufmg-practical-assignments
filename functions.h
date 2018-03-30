#ifndef VERTEX
#include "graph.h"
#define VERTEX 1
#endif

/* Read the train dataset, and returns a graph with users and itens */
Graph readData(char *train);

/* For all u in set of Users, it calculates the mean of all rates from u.
   It is used to normalize the user rating. */
void calculateMeans(Graph &G);

/* Calculates the similarity between two itens (i, j) */
double sim(Graph G, string i, string j);