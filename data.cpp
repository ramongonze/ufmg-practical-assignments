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

Vs split(string s, char delimiter, char delimiterIgnore){
	Vs tokens;
  string word = "";
  bool beginDelimiterIgnore = false;
  for(char& c : s) {
    if((c != delimiter && c != delimiterIgnore) || (c == delimiter && beginDelimiterIgnore == true)){
      word += c;
    } else if(c == delimiter && beginDelimiterIgnore == false){
      tokens.push_back(word);
      word = "";
    } else if(c == delimiterIgnore){
      if(beginDelimiterIgnore == true){
        beginDelimiterIgnore = false;
      } else {
        beginDelimiterIgnore = true;
      }
    }
  }

	return tokens;
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
		av_rating = stod(tokens[13]);

		for(vector<string>::iterator it = authors.begin(); it != authors.end(); it++){
			G[id].authors.insert(*it);
		}

		G[id].av_rating = av_rating;
	}

	*start = G.size();
	content.close();
	return G;
}
