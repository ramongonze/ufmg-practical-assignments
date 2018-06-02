#ifndef _PREDICTION_
#include "prediction.hpp"
#endif

double negPecentage(Graph &G, int user){
	return 100 * ((double)(G[user].neg_neighboors.size())/G[user].neighboors.size());
}

double itemSim(Graph &G, int b1, int b2){
	double sum, s;
	int s1, s2;

	// Take the movie with the smaller amount of users in its adjacent list
	if(G[b1].neighboors.size() < G[b2].neighboors.size()){
		s1 = b1; s2 = b2;
	}else{
		s1 = b2; s2 = b1;		
	}

	sum = 0;
	for(AdjListIt u1 = G[s1].neighboors.begin(); u1 != G[s1].neighboors.end(); u1++)
		if(G[s2].neighboors.find(u1->first) != G[s2].neighboors.end())
			sum += G[s1].neighboors[u1->first] * G[s2].neighboors[u1->first];

	s = sum/(G[b1].sig * G[b2].sig);

	return s;
}

UserRank predictItemBased(Graph &G, Graph &G2, Similarities &S, int user, int type){
	Vdi sim;
	AdjListIt cur, end;

	if(type == 1){
		cur = G[user].neg_neighboors.begin();
		end = G[user].neg_neighboors.end();
	}else{
		cur = G[user].neighboors.begin();
		end = G[user].neighboors.end();
	}

	for(AdjListIt v = G2[user].neighboors.begin(); v != G2[user].neighboors.end(); v++){
		int b1 = v->first;
		double num = 0;
		double den = 0;

		while(cur != end){
			int b2 = cur->first;

			if(S[b1].find(b2) == S[b1].end()){
				// The similarity between b1 and b2 has was not calculated.
				double s = itemSim(G,b1,b2);
				S[b1][b2] = s;
				S[b2][b1] = s;
			}

			num += S[b1][b2] * cur->second;
			den += S[b1][b2];

			cur++;
		}

		sim.push_back(make_pair((num/den + G[b1].av_rating)/2, b1));
	}

	if(type == 1){
		sort(sim.begin(), sim.end());
	}else{
		sort(sim.rbegin(), sim.rend());
	}

	UserRank r;
	for(unsigned int i = 0; i < sim.size(); i++)
		r.push_back(sim[i].second);

	cout << "user:" << user;
	for(unsigned int i = 0; i < r.size(); i++)
		cout << " |" << r[i] << "|";
	cout << endl;

	return r;
}

double contentSim(Graph &G, int b1, int b2);
