#ifndef _GRAPH_
#include "graph.h"
#endif

void addEdge(Graph &G, int u, int v, int weight){
	if(G.find(u) == G.end()){
		// The vertex u is not in the graph
		G[u].neighboors[v] = weight;
		G[u].av_rating = weight;
		G[u].sig = (weight * weight);
	}else{
		G[u].neighboors[v] = weight;
		G[u].av_rating += weight;
		G[u].sig += (weight * weight);
	}
}