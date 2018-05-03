#ifndef _PREDICT_
#include "predict.h"
#endif

using namespace std;

int main(int argc, char *argv[]){
	MapDescription M; // Movies contents
	MapDescription U; // Users contents
	Dictionary D; // Dictionary for words and sets
	Ratings R; // Ratings for Users X Movies
	string buffer;
	ifstream targets;

	if(argc != 4){
		printf("Invalid number of arguments!!\n");
		exit(-1);
	}

	readContent(M, &D, argv[1]);
	readRatings(R, M, U, argv[2]);

/*
	cout << "USER: ";
	for(Setit m = U["u0026762"].G.begin(); m != U["u0026762"].G.end(); m++)
		cout << *m << "  ";
	cout << endl;

	cout << "MOVIE: ";
	for(Setit m = M["i2171847"].G.begin(); m != M["i2171847"].G.end(); m++)
		cout << *m << "  ";
	cout << endl;

	cout << "SIM: " << sim("u0026762", "i2171847", U, M, 'G') << endl;
*/

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
		p = predict(user, movie, R, M, &D, U);

		cout << user << ":" << movie << ',' << setprecision(60) << p << endl;
	}

	targets.close();

	return 0;
}