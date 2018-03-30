#include "graph.h"

#define VERTEX 1

void addEdge(Graph &G, string u, string v, int weight){
	G[u].Adj[v] = weight;
}