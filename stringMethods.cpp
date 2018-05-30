#include "stringMethods.hpp"

using namespace std;

string fixString(string s){
	if(s[0] == ' ')
		s = s.substr(1,s.size());

	for(unsigned int i = 0; i < s.size(); i++)
		if(ispunct(s[i]))
			s.erase(i--, 1);
		else
			s[i] = tolower(s[i]);

	return s;
}

vector<string> split(string s, char delimiter, char delimiterIgnore){
	vector<string> tokens;
  string word = "";
  bool beginDelimiterIgnore = false;
  for(char& c : s) {
    if((c != delimiter && c != delimiterIgnore) || (c == delimiter && beginDelimiterIgnore == true)){
      word += c;
    } else if(c == delimiter && beginDelimiterIgnore == false){
      tokens.push_back(word);
      word = "";
    } else if(c == delimiterIgnore){
      if(beginDelimiterIgnore == true){
        beginDelimiterIgnore = false;
      } else {
        beginDelimiterIgnore = true;
      }
    }
  }

	return tokens;
}

vector<string> split(const string &s, char delimiter){
	vector<string> tokens;
	string token;
	istringstream tokenStream(s);

	while (getline(tokenStream, token, delimiter)){
		tokens.push_back(token);
	}

	return tokens;
}
