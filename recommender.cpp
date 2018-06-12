#include <iostream>
#ifndef _DATA_
#include "data.hpp"
#endif

#ifndef _PREDICTION_
#include "prediction.hpp"
#include "evaluate.hpp"
#endif

int main(){

	Ranks R;
	Graph G;
	Graph G2; // Set of pairs <user,book> to predict rates.
	Similarities S; // Matrix of similarities between books with approach item-based.
	int start;

	G = readContent(&start);
	readRatings(G, G2, start);

	Ranks R2, R3;
	for(GraphIt v = G2.begin(); v != G2.end(); v++){
		int u = v->first;
		if(negPercentage(G,u) >= NEG_PERCENTAGE){
			// If the user has more negative feedback than positive
			if(G2[u].neighboors.size() > 0){ // Check if the 
				R[u] = predictItemBased(G,G2,S,u,1);
				// Rerank based in the positive books content.
				reRank(G,u,R[u]);
				R2[u] = predictItemBased(G,G2,S,u,2);
				R3[u] = predictContentBased(G,G2,u);
			}
		}
	}

	cout << "--COMMON CONTENT-BASED--" << endl;
	evaluate(R3, G2);
	cout << "--COMMON ITEM-BASED--" << endl;
	evaluate(R2, G2);
	cout << "--HEURISTIC--" << endl;
	evaluate(R, G2);

	return 0;
}