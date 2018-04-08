#include <iostream>
#include <string>
#include <map>

using namespace std;

typedef struct Vertex{
	map<string, int> Adj; // The key is the vertex's name, and the value is the edge's weight
	double mean; // Mean rating for all User X Item (Item X User)
}Vertex;

typedef map<string, Vertex> Graph;

// Add and edge = (u, v)
void addEdge(Graph &G, string u, string v, int weight);