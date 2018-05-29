#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

#include <vector>
#ifndef _GRAPH_
#include "graph.h"
#endif

#define _DATA_
/* ------------- Dataset ------------ */
#define RATINGS "DataSet/ratings.csv" 
#define CONTENT "DataSet/books.csv"
#define BOOK_TAGS "DataSet/book_tags.csv"
/* ---------------------------------- */

using namespace std;

typedef vector<string> Vs;

// Split a string in a delimiter
Vs split(const string& s, char delimiter);

// Put all letters in lowercase and remove all punctuations from a given string.
string fixString(string s);

// Read tags for all books, and insert them into a set of tags of a book.
Graph readBookTags();

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

// Read ratings given by users to books, and build a graph.
void readRatings(Graph &G, int start);
