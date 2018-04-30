#ifndef _PREDICT_
#include "predict.h"
#endif

using namespace std;

int main(int argc, char *argv[]){

	movieContent M;

	if(argc != 3){
		printf("Invalid number of arguments!!\n");
		exit(-1);
	}

	readContent(M, argv[1]);

	return 0;
}