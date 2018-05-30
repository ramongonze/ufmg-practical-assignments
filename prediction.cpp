#ifndef _PREDICTION_
#include "prediction.hpp"
#endif

double itemSim(Graph &G, int b1, int b2){
	double sum, s;
	int s1, s2;

	// Take the movie with the smaller amount of users in its adjacent list
	if(G[b1].neighboors.size() < G[b2].neighboors.size()){
		s1 = b1; s2 = b2;
	}else{
		s1 = b2; s2 = b1;		
	}

	sum = 0;
	for(AdjListIt u1 = G[s1].neighboors.begin(); u1 != G[s1].neighboors.end(); u1++)
		if(G[s2].neighboors.find(u1->first) != G[s2].neighboors.end())
			sum += G[s1].neighboors[u1->first] * G[s2].neighboors[u1->first];

	s = sum/(G[b1].sig * G[b2].sig);

	return s;
}

double contentSim(Graph &G, int b1, int b2);
