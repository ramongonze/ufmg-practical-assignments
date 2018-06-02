#include "stringMethods.hpp"
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <fstream>
#include <iostream>

#ifndef _GRAPH_
#include "graph.hpp"
#endif

#define _DATA_
#define EVALUATION_SIZE 70 // Percentage of the data set used to evaluate the recommender system
#define RankSize 10

/* ------------- Dataset ------------ */
#define RATINGS "DataSet/ratings.csv"
#define ANSWERS "DataSet/answers.csv"
#define CONTENT "DataSet/books.csv"
#define PREDICTIONS "Output/predictions.csv"
#define BOOK_TAGS "DataSet/book_tags.csv"
/* ---------------------------------- */

using namespace std;

typedef map<int, int> Mii;

// Read tags for all books, and insert them into a set of tags of a book.
void readBookTags(Mii &IDS, Graph &G);

/*
	- Read books content from the dataset. The content contains:
		-> Authors
		-> Tags
		-> Average rate

	- The function also returns the total number of books in the variable
	  start. It is used to build the graph, modifiyng users' ids.
	  (View description on graph.h).
*/
Graph readContent(int *start);

/* 
	Read ratings given by users to books, and build a graph G (view graph structure in grap.h).
	The graph G2 contains the test set, that is, pairs (user,book) used in the recommender system
	to generate ranks. In the end, the rates of each (user,book) from G2 are used to calculate
	errors between the real rank and the rank generated by the recommender system.
*/
void readRatings(Graph &G, Graph &G2, int start);
