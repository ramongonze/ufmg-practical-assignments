#include "functions.h"

Graph readData(char *train, set<string> &itens){
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

			itens.insert(item);

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

		if(v->second.Adj.size() == 0){
			v->second.mean = (double)0;
		}else{
			v->second.mean = (double)(sum/v->second.Adj.size());
		}
	}
}

double pearsonCorrelation(Graph G, string i, string j){
	double similarity, sum, sig1, sig2;
	set<string> Uij; 

	for(map<string, int>::iterator i1 = G[i].Adj.begin(); i1 != G[i].Adj.end(); i1++){
		if(G[j].Adj.find(i1->first) != G[j].Adj.end()){
			Uij.insert(i1->first);
		}
	}

	/* Uij = Set of all users who have rated both itens i and j */
	sum = sig1 = sig2 = 0;
	for(set<string>::iterator u = Uij.begin(); u != Uij.end(); u++){ /* For each user in Uij */
		sum += -(G[*u].mean)*(G[*u].Adj[i] * G[*u].Adj[j]);
		sig1 += pow((G[*u].Adj[i] - G[*u].mean), 2);
		sig2 += pow((G[*u].Adj[j] - G[*u].mean), 2);
	}

	sig1 = sqrt(sig1);
	sig2 = sqrt(sig2);

	if((sig1*sig2) == 0){
		similarity = 0;
	}else{
		similarity = sum/(sig1 * sig2);
	}

	return similarity;
}

double predictRate(Graph G, string user, string item){
	int k, n;
	double sum, sum2, prediction;
	vector< pair<double, string> > rates;

	k = 20; /* Minimum number of neighbor an item needs to calculate the mean-centering */
	n = 2; /* n nearest neighbors of an item */
	if(G[user].Adj.size() < k){
		/* There are few neighbors. So calculate using CF approach: average rating */
		sum = 0;
		for(map<string, int>::iterator i = G[item].Adj.begin(); i != G[item].Adj.end(); i++){
			sum += i->second;
		}
		if(G[item].Adj.size() == 0){
			prediction = 0;
		}else{
			prediction = sum/G[item].Adj.size();
		}
	}else{
		/* There are at least k neighbors */
		for(map<string, int>::iterator j = G[user].Adj.begin(); j != G[user].Adj.end(); j++){
			/* For each item that the user has rated, calculates the similarity between item and j, with item != j */
			if(item != j->first){
				rates.push_back(make_pair(pearsonCorrelation(G, item, j->first), j->first));
			}
		}
				
		sort(rates.rbegin(), rates.rend());
		sum = sum2 = 0;
		for(int i = 0; i < n; i++){
			sum += rates[i].first * G[user].Adj[rates[i].second];
			sum2 += abs(rates[i].first);
		}
		prediction = sum/sum2;
	}

	return prediction;
}
