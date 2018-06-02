#include <map>
#include <set>
#include <string>
#include <vector>

#define _GRAPH_

using namespace std;

typedef set<int> Si;
typedef set<int>::iterator SiIt;
typedef set<string> Ss;
typedef set<string>::iterator SsIt;

typedef map<int, int> AdjList; // Representation of an adjacent list of a vertex. Key: vertex id. Value: rating.
typedef map<int, int>::iterator AdjListIt; // Iterator of an adjacent list.

typedef struct Vertex{
	/*
		- A vertex represents an user or a book. The graph of users and books is bipartite,
		  where the set V of vertexes can be divided in two disjoint sets:
			U: Set of users
			B: Set of books

		- The edges between users and books represent the rate given by an user to a book.
		  The rates can be 1, 2, 3, 4 or 5.

		- The id for a book is an integer, from 1 to #books.
		- The id for an user is also an integer, from #books+1 to #users.
		  For example: User with id=2 will have his id equals to #books+2

		- Each user/book has an adjacent list (neighboors), that is, which books were read
		  (when the vertex is an user) or which users read it (when the vertex is a book).
	*/
	int id;
	AdjList neighboors;
	
	/* ---------- BOOKS CONTENT ---------- */
	double sig; // Sigma factor, used to calculate the similarity between 2 books
	double av_rating; // Average rating given to a book.
	string series; // Name of the serie which the book belongs. For example: A Harry Potter
				   // book belongs to serie "harry potter" (there are 8 books in total).
	Ss authors; // Authors of a book.
	Si tags; // Tags which describe the book.
	/* ----------------------------------- */

	/* ---------- USERS CONTENT ---------- */
	/*
		- The struct of an user is the union of all content from books that this user read.
		  This struct can be divided in:
			-> Books rated positively (rate 4 or 5);
			-> Books rated negatively (rate 1, 2 or 3).
		
		- The attributes with prefix 'pos_' refer to books rated positively and
		  the ones with prefix 'neg_' refer to books rated negatively.
	*/

	// Books rated posetively
	AdjList pos_neighboors;
	Ss pos_authors;
	Si pos_tags;
	Ss pos_series;

	// Books rated negatively
	AdjList neg_neighboors;
	Ss neg_authors;
	Si neg_tags;
	Ss neg_series;
	/* ----------------------------------- */
}Vertex;

typedef map<int, Vertex> Graph; // Graph representation. Key: user/book id. Value: user/book description
typedef map<int, Vertex>::iterator GraphIt; // Iterator of a graph

// Add an edge = (u, v)
void addEdge(Graph &G, int u, int v, int weight);