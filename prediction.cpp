#include "prediction.h"

Graph readData(char *train){
	FILE *input;
	char buffer[MAX_STR_LENGTH], user[MAX_STR_LENGTH], item[MAX_STR_LENGTH];
	int rating;
	Graph G;

	input = fopen(train, "r");	
	if(!fscanf(input, "%s", buffer)){ // Ignores the header
		printf("Error reading the file!\n");
		return G;
	}

	while(fscanf(input, "%s", buffer) != EOF){
		if(buffer[0] != '\n'){
			strcpy(user, strtok(buffer, ":"));
			strcpy(item, strtok(NULL, ","));
			rating = atoi(strtok(NULL, ","));
			strtok(NULL, "\n");

			addEdge(G, user, item, rating);
			addEdge(G, item, user, rating);
		}
	}

	for(Gi i = G.begin(); i != G.end(); i++){
		G[i->first].mean /= G[i->first].Adj.size();

		if((i->first)[0] == 'i'){
			if(G[i->first].sig > 0)
				G[i->first].sig = sqrt(G[i->first].sig);
			else
				G[i->first].sig = 1;
				
		}
	}

	fclose(input);
	return G;
}

double sim(Graph &G, string i, string j){
	double sum, s;
	string s1,s2;
	vector<double> similarities;

	// Take the item with the smaller amount of users in its adjacent list
	if(G[i].Adj.size() < G[j].Adj.size()){
		s1 = i; s2 = j;
	}else{
		s1 = j; s2 = i;		
	}

	sum = 0;
	for(AdjListi u1 = G[s1].Adj.begin(); u1 != G[s1].Adj.end(); u1++)
		if(G[s2].Adj.find(u1->first) != G[s2].Adj.end())
			sum += u1->second * G[s2].Adj[u1->first];

	s = sum/(G[i].sig * G[j].sig);

	return s;
}

double predict(Graph &G, string user, string i, SimMatrix &M){
	vector<pair<double, int> > S;
	double prediction, sum, norm;

	for(AdjListi j = G[user].Adj.begin(); j != G[user].Adj.end(); j++){
		if(M.find(i) == M.end()){ // If true, the similarity bertween the item i and any other item has never been calculated before
			M[i][j->first] = sim(G, i, j->first);
			M[j->first][i] = M[i][j->first];
		}else if(M[i].find(j->first) == M[i].end()){ // If true, the similarity between the itens i and j has never been calculated before
			M[i][j->first] = sim(G, i, j->first);
			M[j->first][i] = M[i][j->first];
		}

		S.push_back(make_pair(M[i][j->first], G[user].Adj[j->first]));
	}

	sort(S.rbegin(), S.rend());

	sum = norm = 0;
	for(int i = 0; i < S.size() && i < NEIGHBORHOOD; i++){
		sum += S[i].first * S[i].second;
		norm += S[i].first;
	}

	if(norm == 0)
		prediction = 5.0;
	else
		prediction = sum/norm;

	return prediction;
}
