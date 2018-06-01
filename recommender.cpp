#include <iostream>
#ifndef _DATA_
#include "data.hpp"
#endif

#ifndef _PREDICTION_
#include "prediction.hpp"
#endif

int main(){

	Graph G;
	Graph G2; // Set of pairs <user,book> to predict rates.
	int start;

	G = readContent(&start);
	readRatings(G, G2, start);

	return 0;
}
