#include <cmath>
#include <queue>
#include <vector>

using namespace std;

typedef pair<int,int> pii;

typedef struct Edge{
	int source, sink;
	double cost;

	Edge(int _source, int _sink, double _cost){
		source = _source, sink = _sink, cost = _cost;
	}
}Edge;

typedef struct Vertex{
	int x,y; // Cordinates in an euclidan space
	double dist; // Used in Dijkstra and A*. Distance between a vertex i and the sink
	bool visited; // Used in DFS
	vector<Edge> adjs; // Adjacent list of a vertex

	Vertex(int _x, int _y, int _dist){
		x = _x, y = _y, dist = _dist;
	}

	Vertex(int _x, int _y){
		x = _x, y = _y;
		dist = 0.0;
		visited = false;
	}

	// Euclidean Distance between two vertices coordinates
	double operator -(const Vertex &v) const{
		return sqrt((x-v.x)*(x-v.x)+(y-v.y)*(y-v.y));
	}
}Vertex;

// Graph representation
typedef vector<Vertex> Graph;

// Dijkstra implementation with a priority queue.
void dijkstra(Graph G, int s, int t);

/*
 * The A* Algorithm implemented here uses the heuristic of euclidean
 * distance between two vertices, where each vertex has coordinates x and y
 */
void aStar(Graph G, int s, int t);