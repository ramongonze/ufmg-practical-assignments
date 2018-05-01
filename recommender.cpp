#ifndef _PREDICT_
#include "predict.h"
#endif

using namespace std;

int main(int argc, char *argv[]){

	Movie M;
	User U;
	Dictionary D;

	if(argc != 3){
		printf("Invalid number of arguments!!\n");
		exit(-1);
	}

	readContent(M, &D, argv[1]);
	readRatings(U, argv[2]);

	return 0;
}