#include <map>
#include <set>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include "rapidjson/document.h"

#include <iostream>

using namespace std;
using namespace rapidjson;

#define _PREDICT_

// Movie description
typedef struct Description{
	/*
		TF = Term frequency
		iDF = Inverse document frequency
	*/

	int year;
	int awards; // Oscars (weight 7), other wins (weight 2) and nominations (weight 1)
	float imdbRating;
	double sigG; // Sigma parameter to calculate cosine similarity for genres
	double sigAD; // Sigma parameter to calculate cosine similarity for actors and directors
	double sigC; // Sigma parameter to calculate cosine similarity for countries
	double sigP; // Sigma parameter to calculate cosine similarity for plots
	set<string> genres;
	set<string> actorsAndDirectors;
	set<string> countries;
	map<string, pair<int, double> > plot; // Each word and its TF and iDF in the plot
}Description;

// Sets of all genres, actors and directors, countries and words in plot.
typedef struct Dictionary{
	map<string, int> plotsWords; // Word and its frequency in all documents
	set<string> genres;
	set<string> actorsAndDirectors;
	set<string> countries;
}Dictionary;

// Ratings feature matrix for movies genres, actors and directors, countries and words in plot
typedef map<string, double> UFeatures;

// All feature matrixes for all users
typedef struct UVectors{
	set<string> G; // Genres
	set<string> AD; // Actors and Directors
	set<string> C; // Countries
	UFeatures P; // Plots
}UVectors;

void getStopWords(set<string> &stopWords);

// Map containing each movie and its descriptions
typedef map<string, Description> Movie;

typedef map<string, UVectors> User;

// Matrix users x movies -> ratings
typedef map<string, map<string, int> > Ratings;

// Put all letters in lowercase and remove all punctuations
string fixWord(string w);

// Read the content file and returns a map, containing movies and their features
void readContent(Movie &movies, Dictionary *D, string c_name);

// Read the matrix Ratingss x Movies -> Ratings
void readRatings(Ratings &R, Movie &M, User &U, string r_name);

// Calculate the TF-IDF (a based weighing approach) for all movies
void calculateTFIDF(Movie &M, Dictionary *D);

// Calculates the cosine between two vectors of genres (type == G), actors and director (type == A),
// Countries (type == C) or Plots (type == P)
double sim(string u, string m, User &U, Movie &M, char type);

// Predict the rate the movie m would receive from user u
void predict(string u, string m, Ratings &R, Movie &M, Dictionary *D, User &U);