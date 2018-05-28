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
#define W_P 0 // Weight for plots in predicting rate

// MapDescription and user description
typedef struct Description{
	float imdbRating; // Average rating, by public
	double sigG; // Sigma parameter to calculate cosine similarity for genres
	double sigADW; // Sigma parameter to calculate cosine similarity for actors, directors and writers
	double sigC; // Sigma parameter to calculate cosine similarity for countries
	Set G; // Set of genres
	Set ADW; // Set of actors, directors and writers
	Set C; // Set of countries
	Words P; // Set of words in the plot.
}Description;

// Map containing each movie or user and its descriptions
typedef map<string, Description> MapDescription;
typedef map<string, Description>::iterator MapDescriptionit;

// Matrix users x movies -> ratings
typedef map<string, Msi > Ratings;
typedef map<string, Msi >::iterator Ratingsit;

// Read the content file and returns a map, containing movies and their features,
// an integer n = number of distinct words found in all movie's plots and a map
// NI containing a word's id and the number of movies it appears
int readContent(MapDescription &M, Mii &NI, string file);

// Read the matrix Ratings x MapDescriptions -> Ratings and calculates users' vectors
void readRatings(Ratings &R, MapDescription &M, MapDescription &U, string file);

// Calculates the cosine between two vectors of genres (type == G), actors, director and writer (type == A),
// Countries (type == C) or Plots (type == P). Returns -1 if the parameter type is wrong
double sim(string u, string m, MapDescription &U, MapDescription &M, int n_words, char type);

// Predict the rate the movie m would receive from user u. If the user has never watched any movie,
// it returns the movie's imdbRating
double predict(string u, string m, Ratings &R, MapDescription &M, MapDescription &U, int n_words);