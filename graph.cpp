#include "graph.h"

#define VERTEX 1

void addEdge(Graph &G, string u, string v, int weight){
	if(G.find(u) == G.end()){
		// The vertex u is not in the graph
		G[u].mean = 0;
	}
	G[u].Adj[v] = weight;
}