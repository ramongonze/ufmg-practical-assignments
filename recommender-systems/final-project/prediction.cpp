#ifndef _PREDICTION_
#include "prediction.hpp"
#endif

double negPercentage(Graph &G, int user){
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
		end = G[user].neg_neighboors.end();
	}else{
		end = G[user].neighboors.end();
	}

	for(AdjListIt v = G2[user].neighboors.begin(); v != G2[user].neighboors.end(); v++){
		int b1 = v->first;
		double num = 0;
		double den = 0;
		
		if(type == 1){
			cur = G[user].neg_neighboors.begin();
		}else{
			cur = G[user].neighboors.begin();
		}

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
	for(unsigned int i = 0; i < sim.size() && i < RANK_SIZE; i++)
		r.push_back(sim[i].second);

	return r;
}

double contentSim(Graph &G, int u, int b){
	double s1 = 0;
	double s2 = 0;

	// Authors
	if(G[u].authors.size() < G[b].authors.size()){
		for(SsIt a = G[u].authors.begin(); a != G[u].authors.end(); a++)
			if(G[b].authors.find(*a) != G[b].authors.end())
				s1++;
		s1 = s1/G[u].authors.size();
	}else{
		for(SsIt a = G[b].authors.begin(); a != G[b].authors.end(); a++)
			if(G[u].authors.find(*a) != G[u].authors.end())
				s1++;
		s1 = s1/G[b].authors.size();
	}

	// Tags
	if(G[u].tags.size() < G[b].tags.size()){
		for(SiIt t = G[u].tags.begin(); t != G[u].tags.end(); t++)
			if(G[b].tags.find(*t) != G[b].tags.end())
				s2++;
		s2 = s2/G[u].tags.size();
	}else{
		for(SiIt t = G[b].tags.begin(); t != G[b].tags.end(); t++)
			if(G[u].tags.find(*t) != G[u].tags.end())
				s2++;
		s2 = s2/G[b].tags.size();
	}

	return (s1*AUTHORS_W + s2*TAGS_W)/(AUTHORS_W+TAGS_W);
}

double contentPosSim(Graph &G, int u, int b){
	double s1 = 0;
	double s2 = 0;

	// Authors
	if(G[u].pos_authors.size() < G[b].authors.size()){
		for(SsIt a = G[u].pos_authors.begin(); a != G[u].pos_authors.end(); a++)
			if(G[b].authors.find(*a) != G[b].authors.end())
				s1++;
		s1 = s1/G[u].pos_authors.size();
	}else{
		for(SsIt a = G[b].authors.begin(); a != G[b].authors.end(); a++)
			if(G[u].pos_authors.find(*a) != G[u].pos_authors.end())
				s1++;
		s1 = s1/G[b].authors.size();
	}

	// Tags
	if(G[u].pos_tags.size() < G[b].tags.size()){
		for(SiIt t = G[u].pos_tags.begin(); t != G[u].pos_tags.end(); t++)
			if(G[b].tags.find(*t) != G[b].tags.end())
				s2++;
		s2 = s2/G[u].pos_tags.size();
	}else{
		for(SiIt t = G[b].tags.begin(); t != G[b].tags.end(); t++)
			if(G[u].pos_tags.find(*t) != G[u].pos_tags.end())
				s2++;
		s2 = s2/G[b].tags.size();
	}

	return (s1*AUTHORS_W + s2*TAGS_W)/(AUTHORS_W+TAGS_W);
}

UserRank predictContentBased(Graph &G, Graph &G2, int user){
	Vdi sim;
	UserRank r;

	for(AdjListIt book = G2[user].neighboors.begin(); book != G2[user].neighboors.end(); book++)
		sim.push_back(make_pair(contentSim(G, user, book->first), book->first));

	sort(sim.rbegin(), sim.rend());

	for(unsigned int i = 0; i < sim.size() && i < RANK_SIZE; i++)
		r.push_back(sim[i].second);

	return r;
}

void reRank(Graph &G, int user, UserRank &R){
	Vdi sim;

	if(G[user].pos_neighboors.size() == 0){
		// The user does not have any positive feedback. The rank continues the same.
		return;
	}

	for(unsigned int i = 0; i < R.size(); i++){
		double rate = contentPosSim(G,user,R[i]);
		if(G[user].pos_series.find(G[R[i]].series) != G[user].pos_series.end()){
			/* The book R[i] belongs to the same series of one of the books rated
			   positively by the user.*/
			rate = rate*SERIES_BOOST;
			if(rate > 1.0) rate = 1;
		}
		sim.push_back(make_pair(rate, R[i]));
	}

	sort(sim.rbegin(), sim.rend());

	for(unsigned int i = 0; i < sim.size(); i++)
		R[i] = sim[i].second;
}