#include "functions.h"

Graph readData(char *train){
	FILE *input;
	char buffer[MAX_STR_LENGTH], user[MAX_STR_LENGTH], item[MAX_STR_LENGTH];
	int rating;
	long long int timestamp;
	Graph G;

	input = fopen(train, "r");	
	
	fscanf(input, "%s", buffer); /* Ignores the header */
	while(fscanf(input, "%s", buffer) != EOF){
		if(buffer[0] != '\n'){
			strcpy(user, strtok(buffer, ":"));
			strcpy(item, strtok(NULL, ","));
			rating = atoi(strtok(NULL, ","));
			timestamp = atol(strtok(NULL, "\n"));

			addEdge(G, user, item, rating);
			addEdge(G, item, user, rating);

			G[user].mean += rating;
			G[item].mean += rating;
		}
	}

	/* In the end of the execution of readData() each attribute "mean" of
	   all vertexes v in G will contains the sum of all rates given (in case
	   of v == user) or received (in case of v == item) by v */

	fclose(input);
	return G;
}

void computeMeans(Graph &G){
	for(Graph::iterator v = G.begin(); v != G.end(); v++){
		v->second.mean /= v->second.Adj.size();
	}
}

double sim(Graph &G, string i, string j){
	double sum, sig1, sig2;
	int k; // Minimum number of neighbors in common
	sum = sig1 = sig2 = k = 0;
	for(map<string, int>::iterator u1 = G[i].Adj.begin(), u2 = G[j].Adj.begin(); u1 != G[i].Adj.end() || u2 != G[j].Adj.end();){
		
		if(u1 != G[i].Adj.end() && G[i].Adj.find(u1->first) != G[i].Adj.end()){
			sig1 += G[i].Adj[u1->first] * G[i].Adj[u1->first];
		}
		
		if(u2 != G[j].Adj.end() && G[j].Adj.find(u2->first) != G[j].Adj.end()){
			sig2 += G[j].Adj[u2->first] * G[j].Adj[u2->first];
		}
		
		if(u1 != G[i].Adj.end() && G[i].Adj.find(u1->first) != G[i].Adj.end() && G[j].Adj.find(u1->first) != G[j].Adj.end()){
			sum += G[i].Adj[u1->first] * G[j].Adj[u1->first];
			k++;
		}

		if(u1 != G[i].Adj.end()) u1++;
		if(u2 != G[j].Adj.end()) u2++;
	}

	if(sig1 > 0 && sig2 > 0 && k >= 20){
		return sum/ (sqrt(sig1) * sqrt(sig2));
	}else{
		return 0.5;
	}
}

void computeSimilarity(Graph &G, string user, string item, map<string, map<string, double> > &M){

	for(map<string, int>::iterator j = G[user].Adj.begin(); j != G[user].Adj.end(); j++){
		if(G.find(item) == G.end()){
			// The item has never been seen
			M[item][j->first] = 0.5;
			M[j->first][item] = 0.5;
		}else if(item != j->first && M[item].find(j->first) == M[item].end()){
			// The similarity between 'item' and 'j' has not been calculated
			M[item][j->first] = sim(G, item, j->first);
			M[j->first][item] = M[item][j->first]; 
		}
	}

}

double predict(Graph &G, string user, string item, map<string, map<string, double> > &M){
	double prediction = 0;

	for(map<string, int>::iterator r = G[user].Adj.begin(); r != G[user].Adj.end(); r++){
		prediction += M[item][r->first] * r->second;
	}

	if(prediction == 0){
		prediction = 5.0;
	}else{
		prediction /= G[user].Adj.size();
	}

	return prediction;
}