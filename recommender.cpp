#include <iostream>
#include <cstring>
#include "functions.h"

#ifndef VERTEX
#include "graph.h"
#define VERTEX 1
#endif

int main(int argc, char *argv[]){
	char train[MAX_STR_LENGTH], test[MAX_STR_LENGTH], buffer[MAX_STR_LENGTH];
	char user[MAX_STR_LENGTH], item[MAX_STR_LENGTH];
	double prediction;
	FILE *target;
	Graph G;
	map<string, vector<string> > selects;
	map<string, map<string, double> > M;
	set<string> users, itens;

	strcpy(train, argv[1]);
	strcpy(test, argv[2]);

	G = readData(train);
	computeMeans(G);

	// Store users, itens and the selects for predictions user->item
	target = fopen(test, "r");
	fscanf(target, "%s", buffer); // Ignores the header
	while(fscanf(target, "%s", buffer) != EOF){ // For each pair (user, item) in the target file
		if(buffer[0] != '\n'){
			strcpy(user, strtok(buffer, ":"));
			strcpy(item, strtok(NULL, "\n"));

			computeSimilarity(G, user, item, M);
			
			prediction = predict(G, user, item, M);

			cout << user << ":" << item << "," << prediction << endl;			
		}
	}
	fclose(target);
/*
	// Prediction
	computeSimilarity(G, users, itens, M);

	cout << "UserId:ItemId,Prediction\n";
	for(map<string, vector<string> >::iterator u = selects.begin(); u != selects.end(); u++){
		for(vector<string>::iterator i = u->second.begin(); i != u->second.end(); i++){
			prediction = 0;
			for(map<string, int>::iterator j = G[u->first].Adj.begin(); j != G[u->first].Adj.end(); j++){
				prediction += M[*i][j->first];
			}

			if(prediction == 0){
				prediction = 5;
			}else{
				prediction /= G[u->first].Adj.size();
			}

			cout << u->first << ":" << *i << "," << prediction << endl;
		}
	}
*/
	return 0;
}