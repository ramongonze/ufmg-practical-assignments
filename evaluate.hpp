#include <cmath>
#include <ctgmath>
#include <algorithm>
#include <utility>
#include <map>

#define _EVALUATE_

using namespace std;
typedef vector<int> UserRank; //lista de book ids, já ordenada
typedef map<int, UserRank> Ranks; //<id do usuario, UserRank>
typedef map<int, UserRank>::iterator RanksIt;

double nDCG(Ranks predictions, Ranks answers, Graph &G2);

Ranks getAnswers(Graph &G2, Ranks &R);

void evaluate(Ranks R, Graph &G2);
