#ifndef _DATA_
#include "data.hpp"
#endif

string getBookSeries(string s){
	int parenthesesPosition = s.find("(");

	if(parenthesesPosition > 0){
		s = s.substr(parenthesesPosition + 1, s.size());

		int commaPosition = s.find(",");

		if(commaPosition > 0){
			Vs tokens = split(s, ',');
			if(tokens.size() > 0){
				return fixString(tokens[0]);
			}else{
				return fixString(s);
			}
		}else{
			int hashPosition = s.find("#");
			if(hashPosition > 0){
				return fixString(s.substr(0, hashPosition));
			}else{
				return fixString(s);
			}
		}
	}else{
		return "";
	}
}

void readBookTags(Mii &IDS, Graph &G){
	ifstream file;
	string buffer;
	Vs tokens;

	file.open(BOOK_TAGS);

	getline(file, buffer); // Ignores the header.
	while(!file.eof()){
		getline(file, buffer);
		if(buffer.size() == 0) break;
		tokens = split(buffer, ',');
		G[IDS[stoi(tokens[0])]].tags.insert(stoi(tokens[1]));
	}

	file.close();
}

Graph readContent(int *start){
	int id;
	Graph G;
	ifstream content;
	string buffer;
	Vs tokens;
	Vs authors;
	Mii IDS; // Map which key = goodreads_book_id and value = book_id.

	content.open(BOOK_CONTENT);

	getline(content, buffer); // Ignores the header
	while(!content.eof()){
		getline(content, buffer);
		if(buffer.size() == 0) break;

		tokens = split(buffer, ',', '"');

		id = stoi(tokens[0]);
		IDS[stoi(tokens[1])] = id;
		authors = split(tokens[7], ',');

		for(unsigned int a = 0; a < authors.size(); a++){
			G[id].authors.insert(fixString(authors[a]));
		}

		G[id].series = getBookSeries(tokens[10]);
		G[id].av_rating = stod(tokens[12]);
	}
	readBookTags(IDS, G);

	*start = G.size();
	content.close();
	return G;
}

void readRatings(Graph &G, Graph &G2, int start){
	// Seed used to separate the data set in 2: training and evaluation.
	srand(1);

	ifstream file;
	string buffer;
	Vs tokens;
	int u, b, r;

	file.open(RATINGS);

	getline(file, buffer); // Ignores the header.
	while(!file.eof()){
		getline(file, buffer);
		if(buffer.size() == 0) break;
		tokens = split(buffer, ',');
		u = stoi(tokens[0]) + start; //user_id
		b = stoi(tokens[1]); //book_id
		r = stoi(tokens[2]); //rating

		if((G[u].neighboors.find(b) == G[u].neighboors.end() && G2.find(u) == G2.end()) ||
		   (G[u].neighboors.find(b) == G[u].neighboors.end() && G2.find(u) != G2.end() && 
		   G2[u].neighboors.find(b) == G2[u].neighboors.end())){ // Ignore duplicated ratings
			if(rand()%100 >= EVALUATION_SIZE){
				/* 30% of the dataset will be used to access the recommender system */
				addEdge(G2,u,b,r);
			}else{
				/* 70% of the dataset will be used as historical rates, in item-based and
				   conten-based approaches.*/
				addEdge(G,u,b,r);
				addEdge(G,b,u,r);

				if(r > THRESHOLD_RATE){
					G[u].pos_authors.insert(G[b].authors.begin(), G[b].authors.end());
					G[u].pos_tags.insert(G[b].tags.begin(), G[b].tags.end());
					G[u].pos_series.insert(G[b].series);
					G[u].pos_neighboors[b] = r;
				}else{
					G[u].neg_authors.insert(G[b].authors.begin(), G[b].authors.end());
					G[u].neg_tags.insert(G[b].tags.begin(), G[b].tags.end());
					G[u].neg_series.insert(G[b].series);
					G[u].neg_neighboors[b] = r;
				}
			}
		}
	}

	for(GraphIt i = G.begin(); i != G.end(); i++){
		if(i->second.sig > 0)
			i->second.sig = sqrt(i->second.sig);
		else
			i->second.sig = 1;
	}

	file.close();
}