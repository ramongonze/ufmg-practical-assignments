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
#define NEIGHBORHOOD 40 // Maximum number of neighbors to calculate the similarity between two itens
#define CONFIDENCE 40 // Weightness similarity between two itens

typedef map<string, map<string, double> > SimMatrix; // Matrix of similarities

// Read the train dataset, and returns a graph with users and itens.
Graph readData(char *train);

// Computes the cosine similarity between itens i and j in the set of users U
double sim(Graph &G, string i, string j);

// Computes the cosine similarity between all users and itens of selects
// for predictions user->i.
double predict(Graph &G, string user, string i, map<string, map<string, double> > &M);