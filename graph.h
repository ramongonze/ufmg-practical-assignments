#include <iostream>
#include <string>
#include <map>

using namespace std;

typedef struct Vertex{
	map<string, int> Adj; // The key is the vertex's name, and the value is the edge's weight
	double mean; // Mean rating for all User X Item (Item X User)
	double sig; // Sigma factor, used to calculate the similarity between 2 itens
}Vertex;

typedef map<string, Vertex> Graph; // Graph representation
typedef Graph::iterator Gi; // Iterator in a graph
typedef map<string, int> AdjList; // Rpresentation of an adjacent list of a vertex
typedef AdjList::iterator AdjListi; // Iterator in an adjacent list

// Add and edge = (u, v)
void addEdge(Graph &G, string u, string v, int weight);

// Flags to compile
// -O4 -Wall -Wextra -Werror -std=c++11 -pedantic