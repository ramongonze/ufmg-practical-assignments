#include "graph.h"

#define VERTEX 1

void addEdge(Graph &G, string u, string v, int weight){
	if(G.find(u) == G.end()){ // If true, the vertex u is not in the graph
		G[u].Adj[v] = weight;
		G[u].mean = weight;
	}else{
		G[u].Adj[v] = weight;
		G[u].mean += weight;
		if(G[u].Adj.size() == 2)
			G[u].mean /= 2;	
		else
			G[u].mean /= 3;	
	}
}