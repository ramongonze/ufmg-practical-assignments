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
	map<int, int> NI; // first: movie's id, second: # Movies the words appears
	map<string, int> Words;

	if(argc != 4){
		printf("Invalid number of arguments!!\n");
		exit(-1);
	}

	readContent(M, NI, Words, argv[1]);
	readRatings(R, M, U, (int)Words.size(), argv[2]);

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
		p = predict(user, movie, R, M, U);

		cout << user << ":" << movie << ',' << setprecision(60) << p << endl;
	}

	targets.close();

	return 0;
}