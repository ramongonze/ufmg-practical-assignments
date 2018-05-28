#include "contentBased.h"

string fixWord(string w){
	if(w[0] == ' ')
		w = w.substr(1,w.size());


	for(unsigned int i = 0; i < w.size(); i++)
		if(ispunct(w[i]))
			w.erase(i--, 1);
		else
			w[i] = tolower(w[i]);

	return w;
}

int readContent(MapDescription &M, Mii &NI, string file){
	string buffer, word;
	ifstream content;
	Document d;
	Set stopWords;
	Word w;
	Msi Words; // First: word; Second: word's id (integer)
	int wCount; // Total number of words found in all movie's plots

	content.open(file.c_str());

	getline(content, buffer); // Ignores the header

	getStopWords(stopWords);
	wCount = 0;
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
			des.imdbRating = atof(d["imdbRating"].GetString());

			// Plots
			for(stringstream s(d["Plot"].GetString()); s >> word; ){
				word = fixWord(word);
				if(stopWords.find(word) == stopWords.end() && word != ""){
					if(Words.find(word) == Words.end())
						Words[word] = ++wCount;

					w.w = word;
					w.id = Words[word];

					if(des.P.find(w.id) == des.P.end()){
						w.TF = 1;
						des.P[w.id] = w;

						if(NI.find(w.id) == NI.end())
							NI[w.id] = 1;
						else
							NI[w.id] += 1;
					}else{
						des.P[w.id].TF += 1;
					}
				}
			}

			// Genres
			for(stringstream s(d["Genre"].GetString()); s >> word; ){
				word = fixWord(word);
				if(word != "") des.G.insert(word);
			}
			des.sigG = sqrt(des.G.size());

			// Actors, Directors and Writers
			for(stringstream s(d["Actors"].GetString()); getline(s, word, ','); ){
				word = fixWord(word);
				if(word != "") des.ADW.insert(word);
			}

			for(stringstream s(d["Director"].GetString()); getline(s, word, ','); ){
				word = fixWord(word);
				if(word != "") des.ADW.insert(word);
			}

			for(stringstream s(d["Writer"].GetString()); getline(s, word, ','); ){
				word = fixWord(word);
				if(word != "") des.ADW.insert(word);
			}
			des.sigADW = sqrt(des.ADW.size());

			// Countries
			for(stringstream s(d["Country"].GetString()); getline(s, word, ','); ){
				word = fixWord(word);
				if(word != "") des.C.insert(word);
			}
			des.sigC = sqrt(des.C.size());

			M[id] = des; // Create a description for movie m in MapDescription M
		}
	}

	calcTFiDF(M, NI);

	content.close();

	return wCount;
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
		
		R[u][m] = r;
		if(r > 4){
			// Genres
			U[u].G.insert(M[m].G.begin(), M[m].G.end());
			// Actors, directors and writers
			U[u].ADW.insert(M[m].ADW.begin(), M[m].ADW.end());
			// Countries
			U[u].C.insert(M[m].C.begin(), M[m].C.end());
		}
		
		// Plot
		for(Wordsit w = M[m].P.begin(); w != M[m].P.end(); w++){
			if(U[u].P.find(w->first) == U[u].P.end()){
				U[u].P[w->first].TFiDF = r * w->second.TFiDF;
			}else{
				U[u].P[w->first].TFiDF += r * w->second.TFiDF;
			}
		}
	}

	ratings.close();

	// Calculates the TFiDF for user's vector, according to Rocchio recommendation
	for(MapDescriptionit u = U.begin(); u != U.end(); u++){
		for(Wordsit w = U[u->first].P.begin(); w != U[u->first].P.end(); w++){
			w->second.TFiDF /= R[u->first].size();
		}
	}

	for(MapDescriptionit u = U.begin(); u != U.end(); u++){
		// Calculates the sigmas parameters for user u
		u->second.sigG = sqrt(u->second.G.size());
		u->second.sigADW = sqrt(u->second.ADW.size());
		u->second.sigC = sqrt(u->second.C.size());
	}
}

double sim(string u, string m, MapDescription &U, MapDescription &M, int n_words, char type){
	double sum = 0;
	switch(type){
		// Cosine similarity
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
		case 'A': // Actors, Directors and Writers
			if(U[u].ADW.size() < M[m].ADW.size()){
				for(Setit ad = U[u].ADW.begin(); ad != U[u].ADW.end(); ad++)
					if(M[m].ADW.find(*ad) != M[m].ADW.end())
						sum++;
				return sum/ U[u].ADW.size();
			}else{
				for(Setit ad = M[m].ADW.begin(); ad != M[m].ADW.end(); ad++)
					if(U[u].ADW.find(*ad) != U[u].ADW.end())
						sum++;
				return sum/ M[m].ADW.size();	
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
		case 'P':
			// Euclidian distance between two vectors
			if(U[u].P.size() < M[m].P.size()){
				for(Wordsit w = U[u].P.begin(); w != U[u].P.end(); w++){
					if(M[m].P.find(w->first) != M[m].P.end()){
						sum += ((w->second.TFiDF-M[m].P[w->first].TFiDF)*
							   (w->second.TFiDF-M[m].P[w->first].TFiDF))/
							   n_words;
					}else{
						sum += (w->second.TFiDF*w->second.TFiDF)/n_words;
					}
						
				}
			}else{
				for(Wordsit w = M[m].P.begin(); w != M[m].P.end(); w++){
					if(U[u].P.find(w->first) != U[u].P.end()){
						sum += ((w->second.TFiDF-U[u].P[w->first].TFiDF)*
							   (w->second.TFiDF-U[u].P[w->first].TFiDF))/
							   n_words;
					}else{
						sum += (w->second.TFiDF*w->second.TFiDF)/n_words;
					}
				}
			}
			return sqrt(sum);
	}

	return -1;
}

double predict(string u, string m, Ratings &R, MapDescription &M, MapDescription &U, int n_words){

	if(R.find(u) == R.end()){
		// The user u has never watched a movie before. Returns the imdbRating of the movie m
		return M[m].imdbRating;
	}else{
		double simG, simADW, simC, simP;
		if(U[u].G.size() > 0)
			simG = sim(u, m, U, M, 0, 'G');
		else simG = 0;
		if(U[u].ADW.size() > 0)
			simADW = sim(u, m, U, M, 0, 'A');
		else simADW = 0;
		if(U[u].C.size() > 0)
			simC = sim(u, m, U, M, 0, 'C');
		else simC = 0;
		if(U[u].P.size() > 0)
			simP = sim(u, m, U, M, n_words, 'P');
		else simP = 0;

		// A mean is calculated between the rate with similarities and the movie's imdbRating
		return ((simG*W_G + simADW*W_ADW + simC*W_C + simP*W_P) 
			    /(W_G + W_ADW + W_C + W_P)*10)*0.2 + M[m].imdbRating*0.8;  
	}
}