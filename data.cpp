#ifndef _DATA_
#include "data.hpp"
#endif

string getBookSeries(string s){
	int parenthesesPosition = s.find("(");

	if(parenthesesPosition > 0){
		Vs tokens = split(s.substr(parenthesesPosition + 1, s.size()), ',');
		if(tokens.size() > 0){
			return fixString(tokens[0]);
		} else {
			return "";
		}
	} else {
		return "";
	}
}

void readRatings(Graph &G, int start){
	ifstream file;
	string buffer;
	Vs tokens;
	int u, v, r;

	file.open(RATINGS);

	getline(file, buffer); // Ignores the header.

	while(!file.eof()){
		getline(file, buffer);
		if(buffer.size() == 0) break;
		tokens = split(buffer, ',');
		u = stoi(tokens[0]);
		v = stoi(tokens[1]) + start;
		r = stoi(tokens[2]);

		addEdge(G,u,v,r);
		addEdge(G,v,u,r);
	}

	for(GraphIt i = G.begin(); i != G.end(); i++){
		if(i->second.sig > 0)
			i->second.sig = sqrt(i->second.sig);
		else
			i->second.sig = 1;
	}

	file.close();
}

void readBookTags(Mii &IDS){
	Graph G;

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

	content.open(CONTENT);

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
		G[id].av_rating = stod(tokens[13]);
	}
	readBookTags(IDS);

	*start = G.size();
	content.close();
	return G;
}
