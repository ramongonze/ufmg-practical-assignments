#include <vector>
#include <sstream>
#include <string>

using namespace std;

typedef vector<string> Vs;

// Put all letters in lowercase and remove all punctuations from a given string.
string fixString(string s);

// Split a string in a delimiter
Vs split(string s, char delimiter, char delimiterIgnore);
Vs split(const string &s, char delimiter);
