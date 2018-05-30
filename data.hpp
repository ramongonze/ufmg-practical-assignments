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
/* ------------- Dataset ------------ */
#define RATINGS "DataSet/ratings.csv"
#define CONTENT "DataSet/books.csv"
#define BOOK_TAGS "DataSet/book_tags.csv"
/* ---------------------------------- */

using namespace std;

typedef map<int, int> Mii;

// Read tags for all books, and insert them into a set of tags of a book.
void readBookTags(Mii &IDS);

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

// Read ratings given by users to books, and build a graph (view graph structure in grap.h).
void readRatings(Graph &G, int start);
