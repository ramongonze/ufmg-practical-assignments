#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <numeric>
#include <cmath>
#include <set>
#include <algorithm>

#ifndef VERTEX
#include "graph.h"
#define VERTEX 1
#endif

#define MAX_STR_LENGTH 200
#define NEIGHBORHOOD 80 // Maximum number of neighbors to calculate the similarity between two movies
#define COLD_START 7.0

typedef map<string, map<string, double> > SimMatrix; // Matrix of similarities

// Read the train dataset, and returns a bipartite graph with users and movies
Graph readData(char *train);

// Computes the cosine similarity between movies 'i' and 'j'
double sim(Graph &G, string i, string j);

// Computes the weighted mean of all movies 'user' has seen and movie 'i'. 
// All similarity computed in sim(i,j) is stored the matrix of similarities M
double predict(Graph &G, string user, string i, SimMatrix &M);
