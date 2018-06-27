#include "algs.hpp"
#include <stdio.h>

void dfs(Graph &G, Graph &G2, stack<int> &S, int u){
	G[u].visited = true;

	// Shuffle the order of neighboors in the adjacent list
	random_shuffle(G[u].adjs.begin(), G[u].adjs.end());

	for(int i = 0; i < G[u].adjs.size(); ++i){
		int v = G[u].adjs[i];
		if(G[v].visited == false){
			G2[u].adjs.push_back(v);
			G2[v].adjs.push_back(u);
			S.push(v);
			dfs(G,G2,S,v);
		}
	}
}

Graph generateMaze(int n, int m){
	// Each vertex (x,y) has neighboors: (x,y-1), (x+1,y), (x,y+1), (x-1,y)

	srand(3);

	Graph G; // All vertex starts with 4 walls around it.
	Graph G2; // Solution. Each wall removed from G is added in G2
	stack<int> S;

	for(int i = 0; i < n; i++){
		int c = n*i;
		for(int j = 0; j < m; j++){
			int u = c + j; // Vertex index
			G.push_back(Vertex(j,i));
			G2.push_back(Vertex(j,i));
			// Left neighboor
			if(j-1 >= 0) G[u].adjs.push_back(c + j-1);

			// Up neighboor
			if(i+1 < n) G[u].adjs.push_back(c+n + j);				

			// Right neighboor
			if(j+1 < m) G[u].adjs.push_back(c + j+1);				
			
			// Down neighboor
			if(i-1 >= 0) G[u].adjs.push_back(c-n + j);	
		}
	}

	// DFS execution
	S.push(0);
	while(S.size() > 0){
 		int u = S.top(); S.pop();
 		dfs(G,G2,S,u);
	}

	return G2;
}

void printMaze(Graph &G, int n, int m){
	/*
	 * Caption:
	 * l -> left wall
	 * u -> up wall
	 * r -> right wall
	 * d -> down wall
	*/

	printf("%d,%d\n", n, m); // Print the size of the maze

	for(int i = 0; i < n*m; i++){
		int l,u,r,d;
		l = u = r = d = 1;

		for(int j = 0; j < G[i].adjs.size(); j++){
			// Find broken walls (generated in DFS)
			int k = G[i].adjs[j];
			if(G[k].x == G[i].x-1) l = 0;
			else if(G[k].y == G[i].y+1) u = 0;
			else if(G[k].x == G[i].x+1) r = 0;
			else if(G[k].y == G[i].y-1) d = 0;
		}

		if(l) printf("%d,%d,%d,%d;", G[i].x, G[i].x, G[i].y, G[i].y+1);
		if(u) printf("%d,%d,%d,%d;", G[i].x, G[i].x+1, G[i].y+1, G[i].y+1);
		if(r) printf("%d,%d,%d,%d;", G[i].x+1, G[i].x+1, G[i].y+1, G[i].y);
		if(d) printf("%d,%d,%d,%d;", G[i].x, G[i].x+1, G[i].y, G[i].y);
	}

	printf("\n");
}

void dijkstra(Graph &G, int s, int t){
	priority_queue<pii, vector<pii>, greater<pii> > pq;
	pq.push(make_pair(0, s));
 
	while(pq.size()){
		pii a = pq.top(); pq.pop();

		int u = a.second;
		
		if(G[u].visited)
			continue;
		
		G[u].visited = true;
		G[u].dist = a.first;

		if(u == t)
			return;
 
		for(int i = 0; i < G[u].adjs.size(); ++i){
			int v = G[u].adjs[i];
			pq.push(make_pair(G[u].dist + 1, v));
		}
	}
}

void aStar(Graph &G, int s, int t){
	priority_queue<pii, vector< pii >, greater< pii > > pq;
	pq.push(make_pair(0, s));
 
	while(pq.size()){
		pii a = pq.top(); pq.pop();
		
		int u = a.second;

		if(G[u].visited)
			continue;
		
		G[u].visited = true;
		G[u].dist = a.first;

		if(u == t)
			return;
 
		for(int i = 0; i < G[u].adjs.size(); ++i){
			int v = G[u].adjs[i];
			pq.push(make_pair(G[u].dist + 1 + (G[v]-G[t]), v));
		}
	}
}