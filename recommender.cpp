#ifndef _PREDICT_
#include "predict.h"
#endif

using namespace std;

int main(int argc, char *argv[]){
	MapDescription M; // Movies contents
	MapDescription U; // Users contents
	Ratings R; // Ratings for Users X Movies
	string buffer;
	ifstream targets;
	Mii NI; // First: word's id; Second: number of movies where the words appears
	int n_words; // First: word; Second: word's id

	if(argc != 4){
		printf("Invalid number of arguments!!\n");
		exit(-1);
	}

	n_words = readContent(M, NI, argv[1]);
	readRatings(R, M, U, argv[2]);

	targets.open(argv[3]);

	getline(targets, buffer); // Ignores the header
	
	cout << "UserId:ItemId,Prediction" << endl;	
	while(!targets.eof()){
		getline(targets, buffer);
		if(buffer.size() == 0 || buffer[0] == '\n')
			break;

		stringstream s(buffer);

		string user, movie;
		getline(s, user, ':');
		getline(s, movie);

		double p;
		// Predict a rate from 'user' to 'movie'
		p = predict(user, movie, R, M, U, n_words);

		cout << user << ":" << movie << ',' << setprecision(60) << p << endl;
	}

	targets.close();

	return 0;
}