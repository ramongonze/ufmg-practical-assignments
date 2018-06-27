#include <cmath>
#include <queue>
#include <stack>
#include <vector>
#include <limits>
#include <stdio.h>
#include <algorithm>

using namespace std;

#define _ALGS_ 
#define INF 10000000 // Infinite

typedef pair<int,int> pii;

typedef struct Vertex{
	int x,y; // Cordinates in an euclidan space
	double dist; // Used in Dijkstra. Distance between a vertex i and the source
	bool visited; // Used in DFS, Dijkstra and A*.
	vector<int> adjs; // Adjacent list of a vertex

	Vertex();

	Vertex(int _x, int _y, int _dist){
		x = _x, y = _y, dist = _dist;
	}

	Vertex(int _x, int _y){
		x = _x, y = _y;
		dist = INF;
		visited = false;
	}

	// Euclidean Distance between two vertices coordinates
	double operator -(const Vertex &v) const{
		return sqrt((x-v.x)*(x-v.x)+(y-v.y)*(y-v.y));
	}
}Vertex;

// Graph representation
typedef vector<Vertex> Graph;

// Depth-first-search
void dfs(Graph &G, Graph &G2, stack<int> &S, int u);

/* Generate a maze n x m, where each cell in the grid is a vertex in a graph.
 * The generated graph has their vertices' indexes sorted according the example below:
 *
 * Example of a maze 3x4:
 *
 * 	  8  9  10 11
 * 	  4  5  6  7
 * 	  0  1  2  3
 *
*/
Graph generateMaze(int n, int m);

/* Generates the coordinates of walls to be printed in a graphic.
 * Format: wall1;wall2;wall3; ...
 *
 * Each wall is a pair of coordinates:
 * x1,x2,y1,y2;x3,x4,y3,y4; ...
*/
void printMaze(Graph &G, int n, int m);

// Dijkstra implementation with a priority queue.
int dijkstra(Graph &G, int s, int t);

/*
 * The A* Algorithm implemented here uses the heuristic of euclidean
 * distance between two vertices, where each vertex has coordinates x and y
 */
int aStar(Graph &G, int s, int t);