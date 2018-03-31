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

/* Read the train dataset, and returns a graph with users and itens */
Graph readData(char *train, set<string> &itens);

/* For all u in set of Users, it calculates the mean of all rates from u.
   It is used to normalize the user rating. */
void calculateMeans(Graph &G);

/* Calculates the similarity between two itens (i, j), using the Pearson's correlation*/
double pearsonCorrelation(Graph G, string i, string j);

/* Predicts the rate from 'user' to 'item' */
double predictRate(Graph G, string user, string item);