#include <iostream>
#include <cstring>
#include "prediction.h"

#ifndef VERTEX
#include "graph.h"
#define VERTEX 1
#endif

int main(int argc, char *argv[]){
	char train[MAX_STR_LENGTH], test[MAX_STR_LENGTH], buffer[MAX_STR_LENGTH];
	char user[MAX_STR_LENGTH], item[MAX_STR_LENGTH];
	double prediction;
	FILE *target;
	Graph G;
	SimMatrix M;

	if(argc != 3){
		printf("Invalid number of arguments!\n");
		return -1;
	}

	strcpy(train, argv[1]);
	strcpy(test, argv[2]);

	G = readData(train);

	// Store users, itens and the selects for predictions user->item
	target = fopen(test, "r");
	if(!fscanf(target, "%s", buffer)){ // Ignores the header
		printf("Error reading the file!\n");
		return -1;
	}

	printf("UserId:ItemId,Prediction\n");
	while(fscanf(target, "%s", buffer) != EOF){ // For each pair (user, item) in the target file
		if(buffer[0] != '\n'){
			strcpy(user, strtok(buffer, ":"));
			strcpy(item, strtok(NULL, "\n"));

			if(G.find(item) == G.end()){ // If true, the item is unknown
				if(G.find(user) != G.end()) // If true, the user is known
					prediction = G[user].mean;
				else
					prediction = COLD_START;
			}else if(G.find(user) == G.end()) // If true, the user is unknown
				prediction = G[item].mean;
			else{
				prediction = predict(G, user, item, M);
			}

			printf("%s:%s,%.30lf\n", user, item, prediction);
		}
	}
	
	fclose(target);

	return 0;
}
