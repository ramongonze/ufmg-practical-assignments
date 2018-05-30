#ifndef _GRAPH_
#include "graph.hpp"
#endif

#ifndef _DATA_
#include "data.hpp"
#endif


/* Computes similarity between books b1 and b2. The variables b1 and b2
   are the book_id of the books. */
double itemSim(Graph &G, int b1, int b2);

/*
	Computes similarity between books b1 and b2 based on their authors, tags and
	if the book belongs to a serie of books (two books in the same serie are very
	similar). The variables b1 and b2 are the book_id of the books.
*/
double contentSim(Graph &G, int b1, int b2);