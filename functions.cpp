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
		}
	}

	fclose(input);
	return G;
}

double sim(Graph &G, string i, string j){
	double sum, sig1, sig2;
	int k; // Minimum number of neighbors in common
	string s1,s2;

	// Take the item with the smaller amount of users in its adjacent list
	if(G[i].Adj.size() < G[j].Adj.size()){
		s1 = i; s2 = j;
	}else{
		s1 = j; s2 = i;		
	}

	sum = sig1 = sig2 = k = 0;
	for(map<string, int>::iterator u1 = G[s1].Adj.begin(), u2 = G[s2].Adj.begin(); u2 != G[s2].Adj.end(); u2++){
		if(u1 != G[s1].Adj.end()){
			sig1  += pow(u1->second - G[u1->first].mean, 2);

			if(G[s2].Adj.find(u1->first) != G[s2].Adj.end()){
				sum += ((u1->second - G[u1->first].mean) * (u2->second - G[u2->first].mean));
				k++;
			}

			u1++;
		}

		sig2 += pow(u2->second - G[u2->first].mean, 2);
	}

	if(sig1 > 0 && sig2 > 0 && k >= 20){
		return sum/ (sqrt(sig1) * sqrt(sig2));
	}else{
		return 0.5;
	}
}

void computeSimilarity(Graph &G, string user, string item, map<string, map<string, double> > &M){

	for(map<string, int>::iterator j = G[user].Adj.begin(); j != G[user].Adj.end(); j++){
		if(G.find(item) == G.end()){ // If true, the item has never been seen
			M[item][j->first] = 0.5;
			M[j->first][item] = 0.5;
		}else if(M[item].find(j->first) == M[item].end()){ // If true, the similarity between 'item' and 'j' has not been calculated
			M[item][j->first] = sim(G, item, j->first);
			M[j->first][item] = M[item][j->first];
		}
	}
}

double predict(Graph &G, string user, string item, map<string, map<string, double> > &M){
	double prediction = 0;

	for(map<string, int>::iterator r = G[user].Adj.begin(); r != G[user].Adj.end(); r++)
		prediction += M[item][r->first] * r->second;

	if(prediction == 0)
		prediction = 5.0;
	else
		prediction /= G[user].Adj.size();

	return prediction;
}