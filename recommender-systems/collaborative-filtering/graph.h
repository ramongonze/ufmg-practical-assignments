#include <iostream>
#include <string>
#include <map>

using namespace std;

typedef struct Vertex{
	map<string, int> Adj; // The key is the vertex's name, and the value is the edge's weight
	double mean; // Mean ratings for all User X Movie (Movie X User)
	double sig; // Sigma factor, used to calculate the similarity between 2 movies
}Vertex;

typedef map<string, Vertex> Graph; // Graph representation
typedef Graph::iterator Gi; // Iterator of a graph
typedef map<string, int> AdjList; // Representation of an adjacent list of a vertex
typedef AdjList::iterator AdjListi; // Iterator of an adjacent list

// Add and edge = (u, v)
void addEdge(Graph &G, string u, string v, int weight);
