#include "predict.h"

string fixWord(string w){
	if(w[0] == ' ')
		w = w.substr(1,w.size());


	for(int i = 0; i < w.size(); i++)
		if(ispunct(w[i]))
			w.erase(i--, 1);
		else
			w[i] = tolower(w[i]);

	return w;
}

void getStopWords(set<string> &stopWords){
	string words[] = {"ourselves", "hers", "between", "yourself", "but", "again",
	"there", "about", "once", "during", "out", "very", "having", "with", "they",
	"own", "an", "be", "some", "for", "do", "its", "yours", "such", "into", "of",
	"most", "itself", "other", "off", "is", "s", "am", "or", "who", "as", "from",
	"him", "each", "the", "themselves", "until", "below", "are", "we", "these",
	"your", "his", "through", "don", "nor", "me", "were", "her", "more", "himself",
	"this", "down", "should", "our", "their", "while", "above", "both", "up", "to",
	"ours", "had", "she", "all", "no", "when", "at", "any", "before", "them", "same",
	"and", "been", "have", "in", "will", "on", "does", "yourselves", "then", "that",
	"because", "what", "over", "why", "so", "can", "did", "not", "now", "under", "he",
	"you", "herself", "has", "just", "where", "too", "only", "myself", "which", "those",
	"i", "after", "few", "whom", "t", "being", "if", "theirs", "my", "against", "a",
	"by", "doing", "it", "how", "further", "was", "here", "than", ""};
	
	for(int i = 0; i < 128; i++)
		stopWords.insert(words[i]);
}

void calculateTFIDF(Movie &M, Dictionary *D){
	for(Movie::iterator m = M.begin(); m != M.end(); m++){
		// For each movie
		for(map<string, pair<int, double> >::iterator p = m->second.plot.begin(); p != m->second.plot.end(); p++){
			// For each word of movie's m plot
			p->second.second = p->second.first * log((double)M.size()/D->plotsWords[p->first]);
			m->second.sigP += p->second.second * p->second.second; 
		}
		m->second.sigP = sqrt(m->second.sigP);
	}
}

void readContent(Movie &M, Dictionary *D, string c_name){
	string buffer, word;
	ifstream content;
	Document d;
	set<string> stopWords;

	content.open(c_name.c_str());

	getline(content, buffer); // Ignores the header

	getStopWords(stopWords);
	while(!content.eof()){
		getline(content, buffer);
		if(buffer.size() == 0 || buffer[0] == '\n'){
			break;
		}

		string id = buffer.substr(0,8);
		string desc = buffer.substr(9); 
		d.Parse(desc.c_str());
		if(!d.HasMember("Error")){
			Description des;
			des.year = atoi(d["Year"].GetString());
			des.imdbRating = atof(d["imdbRating"].GetString());

			// Awards
			string aw = d["Awards"].GetString();
			des.awards = 0;
			int p1, p2, p3;
			stringstream s(aw);
			if(aw.size() > 25){
				s >> p1 >> p2 >> p3;
				des.awards += 7*p1 + 2*p2 + p3; // Oscars, wins and nominations.
			}else if(aw.size() > 15){
				s >> p1 >> p2;
				des.awards += 2*p1 + p2; // Wins and nominations
			}else if(aw.size() > 5){
				s >> p1;
				des.awards += p1; // Nominations
			}

			// Genres
			des.sigG = 0;
			for(stringstream s(d["Genre"].GetString()); s >> word; ){
				word = fixWord(word);
				if(word != ""){
					des.genres.insert(word);
					D->genres.insert(word);
					des.sigG += 1;
				}
			}
			des.sigG = sqrt(des.sigG);

			// Actors and Directors
			des.sigAD = 0;
			for(stringstream s(d["Actors"].GetString()); getline(s, word, ','); ){
				word = fixWord(word);
				if(word != ""){
					des.actorsAndDirectors.insert(word);
					D->actorsAndDirectors.insert(word);
					des.sigAD += 1;
				}
			}

			for(stringstream s(d["Director"].GetString()); getline(s, word, ','); ){
				word = fixWord(word);
				if(word != ""){
					des.actorsAndDirectors.insert(word);
					D->actorsAndDirectors.insert(word);
					des.sigAD += 1;
				}
			}
			des.sigAD = sqrt(des.sigAD);

			// Countries
			des.sigC = 0;
			for(stringstream s(d["Country"].GetString()); getline(s, word, ','); ){
				word = fixWord(word);
				if(word != ""){
					des.countries.insert(word);
					D->countries.insert(word);
					des.sigC += 1;
				}
			}
			des.sigC = sqrt(des.sigC);

			// Plot
			for(stringstream s(d["Plot"].GetString()); s >> word; ){
				word = fixWord(word);
				if(stopWords.find(word) == stopWords.end()){
					if(des.plot.find(word) == des.plot.end()){
						des.plot[word].first = 1;
						des.plot[word].second = 0.0;
						
						if(D->plotsWords.find(word) == D->plotsWords.end()){
							D->plotsWords[word] = 1;
						}else{
							D->plotsWords[word]++;
						}
					}else{
						des.plot[word].first++;
					}

				}
			}
			des.sigP = 0;

			M[id] = des;
		}
	}

	content.close();

	calculateTFIDF(M, D);
}

void readRatings(Ratings &R, Movie &M, User &U, string r_name){
	int r;
	ifstream ratings;
	string u, m, buffer;

	ratings.open(r_name.c_str());

	getline(ratings, buffer); // Ignores the header

	while(!ratings.eof()){
		getline(ratings, buffer);
		if(buffer.size() == 0 || buffer[0] == '\n'){
			break;
		}
		stringstream s(buffer);
		getline(s, u, ':');
		getline(s, m, ',');
		s >> r;
		R[u][m] = r;
		U[u].G.insert(M[m].genres.begin(), M[m].genres.end());
		U[u].AD.insert(M[m].actorsAndDirectors.begin(), M[m].actorsAndDirectors.end());
		U[u].C.insert(M[m].countries.begin(), M[m].countries.end());
	}

	ratings.close();
}

double sim(string u, string m, User &U, Movie &M, char type){
	double sum = 0;
	switch(type){
		case 'G': // Genres
			if(U[u].G.size() < M[m].genres.size()){
				for(set<string>::iterator g = U[u].G.begin(); g != U[u].G.end(); g++)
					if(M[m].genres.find(*g) != M[m].genres.end())
						sum++;					
			}else{
				for(set<string>::iterator g = M[m].genres.begin(); g != M[m].genres.end(); g++)
					if(U[u].G.find(*g) != U[u].G.end())
						sum++;
			}

			return sum;
			break;
		case 'A': // Actors and Directors

			break;
		case 'C': // Countries

			break;
		case 'P': // Plots

			break;
	}
}

void predict(string u, string m, Ratings &R, Movie &M, Dictionary *D, User &U){

}