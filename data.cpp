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

		G[u].neighboors[v] = r;
		G[v].neighboors[u] = r;
	}

	file.close();
}

Graph readBookTags(){
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
		G[stoi(tokens[0])].tags.insert(stoi(tokens[1]));
	}

	file.close();
	return G;
}

Graph readContent(int *start){
	int id;
	Graph G;
	ifstream content;
	string buffer;
	Vs tokens;
	Vs authors;

	content.open(CONTENT);

	G = readBookTags();

	getline(content, buffer); // Ignores the header
	while(!content.eof()){
		getline(content, buffer);
		if(buffer.size() == 0) break;

		tokens = split(buffer, ',', '"');

		id = stoi(tokens[0]);
		authors = split(fixString(tokens[7]), ',');

		for(vector<string>::iterator it = authors.begin(); it != authors.end(); it++){
			G[id].authors.insert(*it);
		}

		G[id].series = getBookSeries(tokens[10]);
		G[id].av_rating = stod(tokens[13]);
	}

	*start = G.size();
	content.close();
	return G;
}
