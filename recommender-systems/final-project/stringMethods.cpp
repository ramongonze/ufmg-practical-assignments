#ifndef _STRING_
#include "stringMethods.hpp"
#endif

using namespace std;

string fixString(string s){
	while(s.size() > 0 && s[0] == ' ')
		s = s.substr(1,s.size());

	for(unsigned int i = 0; i < s.size(); i++)
		if(ispunct(s[i]) || (i < s.size()-1 && s[i] == ' ' && s[i+1] == ' '))
			s.erase(i--, 1);
		else
			s[i] = tolower(s[i]);
		
	if(s[s.size()-1] == ' ')
		s.erase(s.size()-1, 1);

	return s;
}

Vs split(const string &s, char delimiter){
	Vs tokens;
	string token;
	istringstream tokenStream(s);

	while (getline(tokenStream, token, delimiter)){
		tokens.push_back(fixString(token));
	}

	return tokens;
}

Vs split(string s, char delimiter, char delimiterIgnore){
	Vs tokens;
	string word = "";
	bool beginDelimiterIgnore = false;

	for(char& c : s){
		if((c != delimiter && c != delimiterIgnore) || (c == delimiter && beginDelimiterIgnore == true)){
			word += c;
		}else if(c == delimiter && beginDelimiterIgnore == false){
			tokens.push_back(word);
			word = "";
		}else if(c == delimiterIgnore){
			if(beginDelimiterIgnore == true){
				beginDelimiterIgnore = false;
			}else{
				beginDelimiterIgnore = true;
			}
		}
	}

	return tokens;
}