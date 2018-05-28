#ifndef _DATA_
#include "data.h"
#endif

string fixString(string s){
	if(s[0] == ' ')
		s = s.substr(1,s.size());

	for(unsigned int i = 0; i < s.size(); i++)
		if(ispunct(s[i]))
			s.erase(i--, 1);
		else
			s[i] = tolower(s[i]);

	return s;
}

Vs split(const string &s, char delimiter){
	Vs tokens;
	string token;
	istringstream tokenStream(s);
	
	while (getline(tokenStream, token, delimiter)){
		tokens.push_back(token);
	}

	return tokens;
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
	float av_rating;
	Graph G;
	ifstream content;
	string buffer, authors;
	Vs tokens;

	content.open(CONTENT);

	G = readBookTags();

	getline(content, buffer); // Ignores the header
	while(!content.eof()){
		getline(content, buffer);
		if(buffer.size() == 0) break;
		tokens = split(buffer, ',');
		
		id = stoi(tokens[0]);
		authors = fixString(tokens[7]);
		av_rating = stod(tokens[12]);

		G[id].authors.insert(authors);
		G[id].av_rating = av_rating;
	}

	*start = G.size();
	content.close();
	return G;	
}