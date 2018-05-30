#include <iostream>
#ifndef _DATA_
#include "data.hpp"
#endif

#ifndef _PREDICTION_
#include "prediction.hpp"
#endif

int main(){

	Graph G;
	int start;

	G = readContent(&start);
	readRatings(G, start);

	return 0;
}
