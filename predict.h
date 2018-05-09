#include <map>
#include <set>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include "rapidjson/document.h"

#include <iostream>
#include <iomanip>

using namespace std;
using namespace rapidjson;

#define _PREDICT_
#define W_G 50 // Weight for genres in predicting rate
#define W_ADW 25 // Weight for actors, directors and writers in predicting rate
#define W_C 25 // Weight for countries in predicting rate

typedef set<string> Set; // Set of movies genres, actors, directors and so on
typedef set<string>::iterator Setit; // Set iterator

// MapDescription and user description
typedef struct Description{
	float imdbRating; // Average rating, by public
	double sigG; // Sigma parameter to calculate cosine similarity for genres
	double sigADW; // Sigma parameter to calculate cosine similarity for actors, directors and writers
	double sigC; // Sigma parameter to calculate cosine similarity for countries
	Set G; // Genres
	Set ADW; // Actors, Directors and Writers
	Set C; // Countries
}Description;

// Map containing each movie or user and its descriptions
typedef map<string, Description> MapDescription;
typedef map<string, Description>::iterator MapDescriptionit;

// Matrix users x movies -> ratings
typedef map<string, map<string, int> > Ratings;
typedef map<string, map<string, int> >::iterator Ratingsit;

// Put all letters in lowercase and remove all punctuations
string fixWord(string w);

// Read the content file and returns a map, containing movies and their features
void readContent(MapDescription &movies, string file);

// Read the matrix Ratingss x MapDescriptions -> Ratings
void readRatings(Ratings &R, MapDescription &M, MapDescription &U, string file);

// Calculates the cosine between two vectors of genres (type == G), actors and director (type == A),
// Countries (type == C) or Plots (type == P). Returns -1 if the parameter type is wrong
double sim(string u, string m, MapDescription &U, MapDescription &M, char type);

// Predict the rate the movie m would receive from user u
double predict(string u, string m, Ratings &R, MapDescription &M, MapDescription &U);