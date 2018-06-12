#include <cmath>
#include <ctgmath>
#include <algorithm>
#include <utility>
#include <map>

#define _EVALUATE_

using namespace std;

typedef vector<int> UserRank; // User's rank. A rank is a list o books.
typedef map<int, UserRank> Ranks; // Map containing ranks of users for every user in the dataset.
typedef map<int, UserRank>::iterator RanksIt; // Ranks iterator

// Normalized Discounted Cumulative Gain. A metric used to measure a raking quality. 
double nDCG(Ranks predictions, Ranks answers, Graph &G2);

// Returns the ideal rank for every user in the dataset. The ideal rank is used to calculate nDCG.
Ranks getAnswers(Graph &G2, Ranks &R);

// Generates the ideal rank and uses it to calculate the nDCG.
void evaluate(Ranks R, Graph &G2);