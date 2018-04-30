#include <map>
#include <cstdio>
#include <fstream>
#include <string>
#include "rapidjson/document.h"

#include <iostream>

using namespace std;
using namespace rapidjson;

#define _PREDICT_
#define MAX_LENGTH_DESC 8000 // Max length for a movie description

// Map containing each movie and its descriptions
typedef map<string, map<string, string> > movieContent;

// Features of a movie (iterator in a rapidjson document)
typedef Value::ConstMemberIterator Features;

// Read the content file and returns a map, containing movies and their features
void readContent(movieContent &movies, string file_content);