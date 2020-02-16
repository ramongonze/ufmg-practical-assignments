#include "graph.h"

int main(){
	TGraph *G;
	long smallest_edge, max_flow;
	int test;

	G = createGraph();
	readGraph(G);

	max_flow = 0;
	test = BFS(G);

	while(test){
		smallest_edge = findSmallestEdge(G);
		max_flow += smallest_edge;
		test = BFS(G);
	}

	printf("%ld\n", max_flow);

	destroyGraph(G);
	return 0;
}	