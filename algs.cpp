#include "algs.hpp"

void dijkstra(Graph G, int s, int t){
	priority_queue<pii, vector<pii>, greater<pii> > pq;
	pq.push(make_pair(0, s));
 
	while(pq.size()){
		pii u = pq.top(); pq.pop();
		
		if(G[u.second].visited)
			continue;
		
		G[u.second].visited = true;
		G[u.second].dist = u.first;

		if(u.second == t)
			return;
 
		for(int i = 0; i < G[u.second].adjs.size(); ++i){
			Edge cn = G[u.second].adjs[i];
			pq.push(make_pair(G[u.second].dist + cn.cost, cn.sink));
		}
	}
}

void aStar(Graph G, int s, int t){
	priority_queue<pii, vector< pii >, greater< pii > > pq;
	pq.push(make_pair(0, s));
 
	while(pq.size()){
		pii u = pq.top(); pq.pop();
		
		if(G[u.second].visited)
			continue;
		
		G[u.second].visited = true;
		G[u.second].dist = u.first;

		if(u.second == t)
			return;
 
		for(int i = 0; i < G[u.second].adjs.size(); ++i){
			Edge cn = G[u.second].adjs[i];
			pq.push(make_pair(G[u.second].dist + cn.cost + (G[cn.sink]-G[t]), cn.sink));
		}
	}
}