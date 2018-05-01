#include <map>
#include <set>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include "rapidjson/document.h"

#include <iostream>

using namespace std;
using namespace rapidjson;

#define _PREDICT_
#define MAX_PERSON_NAME 256

// Movie description
typedef struct Description{
	/*
		TF = Term frequency
		iDF = Inverse document frequency
	*/

	int year;
	int awards; // Oscars (weight 7), other wins (weight 2) and nominations (weight 1)
	float imdbRating;
	set<string> genres;
	set<string> actorsAndDirectors;
	set<string> countries;
	map<string, pair<int, float> > plot; // Each word and its TF and iDF in the plot
}Description;

// Sets of all genres, actors and directors, countries and words in plot.
typedef struct Dictionary{
	map<string, int> plotsWords;
	set<string> genres;
	set<string> actorsAndDirectors;
	set<string> countries;
}Dictionary;

void getStopWords(set<string> &stopWords);

// Map containing each movie and its descriptions
typedef map<string, Description> Movie;

// Matrix users x movies -> ratings
typedef map<string, map<string, int> > User;

// Put all letters in lowercase and remove all punctuations
string fixWord(string w);

// Read the content file and returns a map, containing movies and their features
void readContent(Movie &movies, Dictionary *D, string c_name);

// Read the matrix Users x Movies -> Ratings
void readRatings(User &users, string r_name);