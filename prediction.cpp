#include "prediction.h"

Graph readData(char *train){
	FILE *input;
	char buffer[MAX_STR_LENGTH], user[MAX_STR_LENGTH], item[MAX_STR_LENGTH];
	int rating;
	long long int timestamp;
	Graph G;

	input = fopen(train, "r");	
	
	fscanf(input, "%s", buffer); // Ignores the header
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

	for(Gi i = G.begin(); i != G.end(); i++)
		if((i->first)[0] == 'i'){
			G[i->first].sig = sqrt(G[i->first].sig);
			if(G[i->first].sig == 0)
				G[i->first].sig = 1;
		}

	fclose(input);
	return G;
}

double sim(Graph &G, string i, string j){
	int c; // Number of users that have watched both i and j
	double sum, s;
	string s1,s2;
	vector<double> similarities;

	// Take the item with the smaller amount of users in its adjacent list
	if(G[i].Adj.size() < G[j].Adj.size()){
		s1 = i; s2 = j;
	}else{
		s1 = j; s2 = i;		
	}

	c = 0;
	for(AdjListi u1 = G[s1].Adj.begin(); u1 != G[s1].Adj.end(); u1++){
		if(G[s2].Adj.find(u1->first) != G[s2].Adj.end()){
			similarities.push_back(u1->second * G[s2].Adj[u1->first]);
			c++;
		}
	}
		

	sort(similarities.rbegin(), similarities.rend());

	sum = 0;
	for(int k = 0; k < similarities.size() && k < NEIGHBORHOOD; k++)
		sum += similarities[k];

	s = (min(c, CONFIDENCE)/CONFIDENCE) * (sum/(G[i].sig * G[j].sig));

	return s;
}

double predict(Graph &G, string user, string i, SimMatrix &M){
	double prediction, sum;

	prediction = sum = 0;
	for(AdjListi j = G[user].Adj.begin(); j != G[user].Adj.end(); j++){
		if(M.find(i) == M.end()){ // If true, the similarity bertween the item i and any other item has never been calculated before
			M[i][j->first] = sim(G, i, j->first);
			M[j->first][i] = M[i][j->first];
		}else if(M[i].find(j->first) == M[i].end()){ // If true, the similarity between the itens i and j has never been calculated before
			M[i][j->first] = sim(G, i, j->first);
			M[j->first][i] = M[i][j->first];
		}

		prediction += (M[i][j->first] * j->second);
		sum += M[i][j->first];
	}

	if(prediction == 0)
		prediction = 5.0;
	else
		prediction /= sum;

	return prediction;
}