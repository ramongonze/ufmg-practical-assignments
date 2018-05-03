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

void getStopWords(Set &stopWords){
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

void calculateTFIDF(MapDescription &M, Dictionary *D){
	for(MapDescriptionit m = M.begin(); m != M.end(); m++){
		// For each movie
		m->second.sigP = 0;
		for(map<string, pair<int, double> >::iterator p = m->second.W.begin(); p != m->second.W.end(); p++){
			// For each word of movie's m plot
			p->second.second = p->second.first * log((double)M.size()/D->WF[p->first]);
			m->second.sigP += p->second.second * p->second.second; 
		}
		m->second.sigP = sqrt(m->second.sigP);
	}
}

void readContent(MapDescription &M, Dictionary *D, string file){
	string buffer, word;
	ifstream content;
	Document d;
	Set stopWords;

	content.open(file.c_str());

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
			for(stringstream s(d["Genre"].GetString()); s >> word; ){
				word = fixWord(word);
				if(word != ""){
					des.G.insert(word);
					D->G.insert(word);
				}
			}
			des.sigG = sqrt(des.G.size());

			// Actors and Directors
			for(stringstream s(d["Actors"].GetString()); getline(s, word, ','); ){
				word = fixWord(word);
				if(word != ""){
					des.AD.insert(word);
					D->AD.insert(word);
				}
			}

			for(stringstream s(d["Director"].GetString()); getline(s, word, ','); ){
				word = fixWord(word);
				if(word != ""){
					des.AD.insert(word);
					D->AD.insert(word);
				}
			}
			des.sigAD = sqrt(des.AD.size());

			// Countries
			for(stringstream s(d["Country"].GetString()); getline(s, word, ','); ){
				word = fixWord(word);
				if(word != ""){
					des.C.insert(word);
					D->C.insert(word);
				}
			}
			des.sigC = sqrt(des.C.size());

			// Plot
			for(stringstream s(d["Plot"].GetString()); s >> word; ){
				word = fixWord(word);
				if(stopWords.find(word) == stopWords.end()){
					if(des.W.find(word) == des.W.end()){
						des.W[word].first = 1;
						des.W[word].second = 0.0;
						
						if(D->WF.find(word) == D->WF.end()){
							D->WF[word] = 1;
						}else{
							D->WF[word]++;
						}
					}else{
						des.W[word].first++;
					}

				}
			}

			M[id] = des; // Create a description for movie m in MapDescription M
		}
	}

	content.close();

	calculateTFIDF(M, D);
}

void readRatings(Ratings &R, MapDescription &M, MapDescription &U, string file){
	int r;
	ifstream ratings;
	string u, m, buffer;

	ratings.open(file.c_str());

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
		
		if(R.find(u) == R.end()){
			// The user has never been read before
			U[u].year = 0;
			U[u].awards = 0;
			U[u].imdbRating = 0;
		}

		R[u][m] = r;
		U[u].G.insert(M[m].G.begin(), M[m].G.end());
		U[u].AD.insert(M[m].AD.begin(), M[m].AD.end());
		U[u].C.insert(M[m].C.begin(), M[m].C.end());
		U[u].W.insert(M[m].W.begin(), M[m].W.end());

		U[u].year += M[m].year;
		U[u].awards += M[m].awards;
		U[u].imdbRating += M[m].imdbRating;
	}

	ratings.close();

	for(MapDescriptionit u = U.begin(); u != U.end(); u++){
		// All user's vectors have not been calculated
		u->second.words = false;

		// Calculates the sigmas parameters for user u
		u->second.sigG = sqrt(u->second.G.size());
		u->second.sigAD = sqrt(u->second.AD.size());
		u->second.sigC = sqrt(u->second.C.size());

		// Calculates the average for year, imdbRating and awards
		// for all movies the user u has watched before
		u->second.year = (int)(u->second.year/R[u->first].size());
		u->second.awards = (int)(u->second.awards/R[u->first].size());
		u->second.imdbRating = u->second.imdbRating/R[u->first].size();
	}
}

double sim(string u, string m, MapDescription &U, MapDescription &M, char type){
	double sum = 0;
	switch(type){
		case 'G': // Genres
			if(U[u].G.size() < M[m].G.size()){
				for(Setit g = U[u].G.begin(); g != U[u].G.end(); g++)
					if(M[m].G.find(*g) != M[m].G.end())
						sum++;
				return sum/ U[u].G.size();
			}else{
				for(Setit g = M[m].G.begin(); g != M[m].G.end(); g++)
					if(U[u].G.find(*g) != U[u].G.end())
						sum++;
				return sum/ M[m].G.size();
			}
		case 'A': // Actors and Directors
			if(U[u].AD.size() < M[m].AD.size()){
				for(Setit ad = U[u].AD.begin(); ad != U[u].AD.end(); ad++)
					if(M[m].AD.find(*ad) != M[m].AD.end())
						sum++;
				return sum/ U[u].AD.size();
			}else{
				for(Setit ad = M[m].AD.begin(); ad != M[m].AD.end(); ad++)
					if(U[u].AD.find(*ad) != U[u].AD.end())
						sum++;
				return sum/ M[m].AD.size();	
			}
		case 'C': // Countries
			if(U[u].C.size() < M[m].C.size()){
				for(Setit c = U[u].C.begin(); c != U[u].C.end(); c++)
					if(M[m].C.find(*c) != M[m].C.end())
						sum++;
				return sum/ U[u].C.size();
			}else{
				for(Setit c = M[m].C.begin(); c != M[m].C.end(); c++)
					if(U[u].C.find(*c) != U[u].C.end())
						sum++;
				return sum/ M[m].C.size();
			}
		case 'P': // Plots
			if(U[u].W.size() < M[m].W.size()){
				for(map<string, pair<int, double> >::iterator w = U[u].W.begin(); w != U[u].W.end(); w++)
					if(M[m].W.find(w->first) != M[m].W.end())
						sum += w->second.second * M[m].W[w->first].second;
			}else{
				for(map<string, pair<int, double> >::iterator w = M[m].W.begin(); w != M[m].W.end(); w++)
					if(U[u].W.find(w->first) != U[u].W.end())
						sum += w->second.second * U[u].W[w->first].second;
			}
			
			return sum/ (U[u].sigP * M[m].sigP);
	}
}

double simYear(int y1, int y2){
	// Each year of difference, the similarity decreases 5%
	int dif = abs(y1-y2);
	return max(0.0, (100.0 - (dif*5))/100);
}

double simAwards(int a1, int a2){
	// Each award of difference, the similarity decreases 2%
	int dif = abs(a1 - a2);
	return max(0.0, (100.0 - (dif*2)/100));
}

double predict(string u, string m, Ratings &R, MapDescription &M, Dictionary *D, MapDescription &U){

	if(R.find(u) == R.end()){
		// The user u has never watched a movie before. Returns the imdbRating of the movie m
		return M[m].imdbRating;
	}else{
		// if(U[u].words == false){
		// 	// It is necesseray to calculate the u's vector
		// 	U[u].sigP = 0;
		// 	for(map<string, pair<int, double> >::iterator w = U[u].W.begin(); w != U[u].W.end(); w++){
		// 		// For each word in user's u plots
		// 		double sum = 0;
		// 		string word = w->first;
		// 		for(map<string, int>::iterator m = R[u].begin(); m != R[u].end(); m++){
		// 			// For each movie watched by u
		// 			string movie = m->first;
		// 			int rate = m->second;
		// 			if(M[movie].W.find(word) != M[movie].W.end())
		// 				sum += rate * M[movie].W[word].second;
		// 		}				
				
		// 		w->second.second = sum/ R[u].size();
		// 		U[u].sigP += w->second.second * w->second.second;
		// 	}

		// 	U[u].sigP = sqrt(U[u].sigP);
		// 	U[u].words = true;
		// }

		/*return ((sim(u, m, U, M, 'G') * GENRE + sim(u, m, U, M, 'A') * ACTOR_DIRECTORS
		 + sim(u, m, U, M, 'C') * COUNTRIES + sim(u, m, U, M, 'P') * PLOT
		 + simYear(U[u].year, M[m].year) * YEAR + simAwards(U[u].awards, M[m].awards) * AWARDS) 
		 / (GENRE+ACTOR_DIRECTORS+COUNTRIES+PLOT+AWARDS+YEAR))*10;*/

		return ((((sim(u, m, U, M, 'G') * GENRE + sim(u, m, U, M, 'A') * ACTOR_DIRECTORS
		 + sim(u, m, U, M, 'C') * COUNTRIES + simYear(U[u].year, M[m].year) * YEAR)
		  / (GENRE+ACTOR_DIRECTORS+COUNTRIES+YEAR)) * 10) + M[m].imdbRating)/2;
	}
}