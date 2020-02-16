#include <vector>
#include <sstream>
#include <string>

using namespace std;

#define _STRING_

typedef vector<string> Vs;

// Put all letters in lowercase and remove all punctuations from a given string.
string fixString(string s);

// Split a string in a delimiter
Vs split(const string &s, char delimiter);

// Split a string in a delimiter ignoring a character
Vs split(string s, char delimiter, char delimiterIgnore);