#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "qsort.h"

typedef struct Numbers{
	int n; // Array size. All arrays are going to have the same size.
	int *bars;
	int *houses;
	int *oddArray;
	int *evenArray;
}Numbers;

// Read bar and house's numbers, and return a 'Numbers' struct
Numbers *readNumbers(int N);

// Greedy algorithm to find the max number of flags
int greedy(Numbers *options);

// The funcion below checks if a number is valid or not in the greedy algorithm
int checkCandidate(Numbers *options, int *solution, int candidate);

// The function below count the distance between a bar (or house) and its pair,
// according their positions in the evenArray and oddArray
int* countDistances(Numbers *options);

// Dynamic algorithm to find the max number of flags
int dynamic(Numbers *options);

// Brute force algorithm to find the max number of flags
int bruteForce(Numbers *options);

// Find the correspondent number of a bar or house
int findCorrespondent(Numbers *options, int A);

// Free all the memory allocated
void destroyNumbers(Numbers *options);