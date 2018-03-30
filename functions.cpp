#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "functions.h"

#define MAX_BUFFER 50

Graph readData(char *train){
	FILE *input;
	char buffer[MAX_BUFFER], user[MAX_BUFFER], item[MAX_BUFFER];
	int rating;
	long long int timestamp;
	Graph G;

	input = fopen(train, "r");	
	
	fscanf(input, "%s", buffer); /* Header */
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

void calculateMeans(Graph &G){
	double sum;

	for(map<string, Vertex>::iterator v = G.begin(); v != G.end(); v++){
		if(v->first[0] == 'u'){ /* Is an user */
			sum = 0;
			for(map<string, int>::iterator u = v->second.Adj.begin(); u != v->second.Adj.end(); u++){
				sum += u->second;
			}
		}

		v->second.mean = sum/v->second.Adj.size();
	}
}

double sim(Graph G, string i, string j){
	
}