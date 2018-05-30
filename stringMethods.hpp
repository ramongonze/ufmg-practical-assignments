#include <vector>
#include <sstream>
#include <string>

using namespace std;

// Put all letters in lowercase and remove all punctuations from a given string.
string fixString(string s);
// Split a string in a delimiter
vector<string> split(string s, char delimiter, char delimiterIgnore);
vector<string> split(const string &s, char delimiter);
