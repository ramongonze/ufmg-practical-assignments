#ifndef _GRAPH_
#include "graph.hpp"
#endif

#ifndef _DATA_
#include "data.hpp"
#endif


/* Computes similarity between books b1 and b2. The variables b1 and b2
   are the book_id of the books. */
double itemSim(Graph &G, int b1, int b2);

double contentSim();