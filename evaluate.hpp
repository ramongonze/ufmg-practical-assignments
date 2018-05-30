#include <algorithm>
#include <utility>
#include <map>

using namespace std;

typedef pair<float,int> BookIdRating; //par que se refere à <'nota','id do livro'>
typedef vector<BookIdRating> rankUser;
typedef map<int,rankUser> answers; //<id do usuario, rankUser>

void evaluate();
