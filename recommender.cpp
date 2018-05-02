#ifndef _PREDICT_
#include "predict.h"
#endif

using namespace std;

int main(int argc, char *argv[]){
	Movie M;
	Ratings R;
	Dictionary D;
	User U;
	string buffer;
	ifstream targets;

	if(argc != 4){
		printf("Invalid number of arguments!!\n");
		exit(-1);
	}

	readContent(M, &D, argv[1]);
	readRatings(R, M, U, argv[2]);

	targets.open(argv[3]);

	getline(targets, buffer); // Ignores the header
	
	cout << "UserId:ItemId,Prediction" << endl;	
	while(!targets.eof()){
		getline(targets, buffer);
		stringstream s(buffer);

		string user, movie;
		getline(s, user, ':');
		getline(s, movie);



	}

	targets.close();

	return 0;
}