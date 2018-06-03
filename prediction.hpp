#ifndef _GRAPH_
#include "graph.hpp"
#endif

#ifndef _DATA_
#include "data.hpp"
#endif

#ifndef _EVALUATE_
#include "evaluate.hpp"
#endif

#define _PREDICTION_
#define NEG_PERCENTAGE 70
#define SERIES_BOOST 1.4 // Gives a boost of 40% to similarity if the books belongs to 
						 // the same series.
#define AUTHORS_W 25 // Authors' weight to calculate similarity between two books
#define TAGS_W 75 // Tags' weight to calculate similarity between two books

// First key is a book id. Each book has a list of its similarities with the other books.
// Matrix book x book -> similarity
typedef map<int, map<int, double> > Similarities;

// Key = Similarity of some book with book B. Value: book B id
typedef vector<pair<double, int> > Vdi;

// Returns the percentagem of books rated negatively.
double negPercentage(Graph &G, int user);

/* Computes similarity between books b1 and b2. The variables b1 and b2
   are the book_id of the books. */
double itemSim(Graph &G, int b1, int b2);

/*
	- Returns a rank for all users, according to pairs <user,book> in G2.

	- The function can return two ranks:
		type = 1 -> return a rank in ascending order of similarities (less similar first);
		type = 2 -> return a rank in descending order of similarities (most similar first);

	- The matrix book x book of similarities is a memorizing technique used to avoid calculate
	  the same value more than once. Each time a similarity between books b1 and b2 is calculated,
	  it is saved in S[b1][b2] and S[b2][b1].
*/
UserRank predictItemBased(Graph &G, Graph &G2, Similarities &S, int user, int type);

/*
	Computes similarity between books b1 and b2 based on their authors, tags and
	if the book belongs to a serie of books (two books in the same serie are very
	similar). The variables b1 and b2 are the book_id of the books.
*/
double contentSim(Graph &G, int b1, int b2);

/*
	- Rerank a user rank according to the similarities with books he rated posetively.
	  This rerank is made based on books content.
	  If the user has no positive feedback, the rank continues the same.
*/
void reRank(Graph &G, int user, UserRank &R);