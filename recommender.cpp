#include <iostream>
#include <cstring>
#include "functions.h"

#ifndef VERTEX
#include "graph.h"
#define VERTEX 1
#endif

int main(int argc, char *argv[]){
	char train[MAX_STR_LENGTH], test[MAX_STR_LENGTH], buffer[MAX_STR_LENGTH];
	char user[MAX_STR_LENGTH], item[MAX_STR_LENGTH];
	double prediction;
	set<string> itens;
	map<string, double> p;
	FILE *target;
	Graph G;
	string name;

	strcpy(train, argv[1]);
	strcpy(test, argv[2]);

	G = readData(train, itens);
	calculateMeans(G);

	/* Prediction */
	cout << "UserId:ItemId,Prediction\n";
	target = fopen(test, "r");
	fscanf(target, "%s", buffer); /* Ignores the header */
	while(fscanf(target, "%s", buffer) != EOF){ /* For each pair (user, item) in the target file */
		if(buffer[0] != '\n'){
			strcpy(user, strtok(buffer, ":"));
			strcpy(item, strtok(NULL, "\n"));
			prediction = predictRate(G, user, item);

			cout << user << ":" << item << "," << prediction << endl;
		}
	}

	fclose(target);

	return 0;
}