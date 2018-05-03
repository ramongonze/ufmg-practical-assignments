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
#define GENRE 70
#define ACTOR_DIRECTORS 25
#define COUNTRIES 5
#define YEAR 0
#define PLOT 25
#define AWARDS 10


typedef set<string> Set; // Set of movies genres, actors, directors and so on
typedef set<string>::iterator Setit; // Set iterator
typedef map<string, int> WordFrequency; // Map to store word's frequency in a plot
typedef map<string, int>::iterator WordFrequencyit; // Word's frequency map iterator

// MapDescription and user description
typedef struct Description{
	// TF = Term frequency
	// iDF = Inverse document frequency
	int year;
	int awards; // Oscars (weight 7), other wins (weight 2) and nominations (weight 1)
	float imdbRating; // Average rating, by public
	double sigG; // Sigma parameter to calculate cosine similarity for genres
	double sigAD; // Sigma parameter to calculate cosine similarity for actors and directors
	double sigC; // Sigma parameter to calculate cosine similarity for countries
	double sigP; // Sigma parameter to calculate cosine similarity for plots
	Set G; // Genres
	Set AD; // Actors and Directors
	Set C; // Countries
	bool words; // Flag to check if the user's vectors have been calculated before or not
	map<string, pair<int, double> > W; // Each word in a plot and its TF and TF-iDF
}Description;

// Sets of all genres, actors and directors, countries and words in plot.
typedef struct Dictionary{
	WordFrequency WF; // Words and their frequency in all documents
	Set G; // Genres
	Set AD; // Actors and Directors
	Set C; // Countries
}Dictionary;

// Map containing each movie or user and its descriptions
typedef map<string, Description> MapDescription;
typedef map<string, Description>::iterator MapDescriptionit;

// Matrix users x movies -> ratings
typedef map<string, map<string, int> > Ratings;
typedef map<string, map<string, int> >::iterator Ratingsit;

// Stores pre selected stop words into a set
void getStopWords(Set &stopWords);

// Put all letters in lowercase and remove all punctuations
string fixWord(string w);

// Read the content file and returns a map, containing movies and their features
void readContent(MapDescription &movies, Dictionary *D, string file);

// Read the matrix Ratingss x MapDescriptions -> Ratings
void readRatings(Ratings &R, MapDescription &M, MapDescription &U, string file);

// Calculate the TF-IDF (a based weighing approach) for all movies
void calculateTFIDF(MapDescription &M, Dictionary *D);

// Calculates the cosine between two vectors of genres (type == G), actors and director (type == A),
// Countries (type == C) or Plots (type == P)
double sim(string u, string m, MapDescription &U, MapDescription &M, char type);

// Calculates the similarity between years
// Each year of difference, the similarity decreases 5%
double simYear(int y1, int y2);

// Calculates the similarity between awards amounts
// Each award of difference, the similarity decreases 2%
double simAwards(int a1, int a2);

// Predict the rate the movie m would receive from user u
double predict(string u, string m, Ratings &R, MapDescription &M, Dictionary *D, MapDescription &U);