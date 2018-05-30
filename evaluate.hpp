#include <algorithm>
#include <utility>
#include <map>

using namespace std;

typedef vector<int> rankUser; //lista de book ids, já ordenada
typedef map<int,rankUser> userRanks; //<id do usuario, rankUser>

void evaluate();
