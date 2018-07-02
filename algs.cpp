#include "algs.hpp"

Graph generateMaze(int n, int m){
	// Each vertex (x,y) has neighboors: (x,y-1), (x+1,y), (x,y+1), (x-1,y)

	int u, v, old_u;
	Graph G; // All vertex starts with 4 walls around it.
	Graph G2; // Solution. Each wall removed from G is added in G2
	stack<int> S, D;

	for(int i = 0; i < n; i++){
		int c = m*i;
		for(int j = 0; j < m; j++){
			int u = c + j; // Vertex index
			G.push_back(Vertex(j,i));
			G2.push_back(Vertex(j,i));
			// Left neighboor
			if(j-1 >= 0) G[u].adjs.push_back(u-1);

			// Up neighboor
			if(i+1 < n) G[u].adjs.push_back(u+m);				

			// Right neighboor
			if(j+1 < m) G[u].adjs.push_back(u+1);				
			
			// Down neighboor
			if(i-1 >= 0) G[u].adjs.push_back(u-m);	

 			random_shuffle(G[u].adjs.begin(), G[u].adjs.end());
		}
	}

	// Iterative Depth-first-search
	S.push(0);
	while(S.size() > 0){
 		u = S.top(); S.pop();
 		
 		while(u != -1){
 			
 			G[u].visited = true;
 				
 			old_u = u; u = -1;
 			for(unsigned int i = 0; i < G[old_u].adjs.size(); ++i){
				v = G[old_u].adjs[i];
				
				if(G[v].visited == false){
					G2[old_u].adjs.push_back(v);
					G2[v].adjs.push_back(old_u);
					S.push(v);
					G[old_u].adjs.erase(G[old_u].adjs.begin());
					u = v;
					break;
				}
			}
 		}
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

	FILE *out;

	out = fopen("out","w");

	fprintf(out,"%d,%d\n", n, m); // Print the size of the maze

	for(int i = 0; i < n*m; i++){
		int l,u,r,d;
		l = u = r = d = 1;

		for(unsigned int j = 0; j < G[i].adjs.size(); j++){
			// Find broken walls (generated in DFS)
			int k = G[i].adjs[j];
			if(G[k].x == G[i].x-1) l = 0;
			else if(G[k].y == G[i].y+1) u = 0;
			else if(G[k].x == G[i].x+1) r = 0;
			else if(G[k].y == G[i].y-1) d = 0;
		}

		if(l) fprintf(out,"%d,%d,%d,%d;", G[i].x, G[i].x, G[i].y, G[i].y+1);
		if(u) fprintf(out,"%d,%d,%d,%d;", G[i].x, G[i].x+1, G[i].y+1, G[i].y+1);
		if(r) fprintf(out,"%d,%d,%d,%d;", G[i].x+1, G[i].x+1, G[i].y+1, G[i].y);
		if(d) fprintf(out,"%d,%d,%d,%d;", G[i].x, G[i].x+1, G[i].y, G[i].y);
	}

	fprintf(out,"\n");
	fclose(out);
}

int dijkstra(Graph &G, int s, int t){
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
			break;
 
		for(unsigned int i = 0; i < G[u].adjs.size(); ++i){
			int v = G[u].adjs[i];
			pq.push(make_pair(G[u].dist + 1, v));
		}
	}

	return (int)G[t].dist;
}

int aStar(Graph &G, int s, int t){
	priority_queue<pii, vector<pii>, greater<pii> > pq;

	pq.push(make_pair(0,s));
	while(pq.size()){
		pii a = pq.top(); pq.pop();
		
		int u = a.second;

		if(G[u].visited)
			continue;
		
		G[u].visited = true;
		G[u].dist = a.first - (G[u]-G[t]);

		if(u == t)
			break;
 
		for(unsigned int i = 0; i < G[u].adjs.size(); ++i){
			int v = G[u].adjs[i];
			pq.push(make_pair(G[u].dist + 1 + (G[v]-G[t]),v));
		}
	}

	return (int)G[t].dist;
}






