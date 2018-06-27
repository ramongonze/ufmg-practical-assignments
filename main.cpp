#ifndef _ALGS_
#include "algs.hpp"
#endif

#include <ctime>

int main(){
	srand(7); // Used in function generateMaze()

	int n = 100;
	int m = 131;
	int d;
	time_t begin, end;

	Graph maze;

	maze = generateMaze(n,m);
	//printMaze(maze,n,m);

	time(&begin);
	d = dijkstra(maze,0,n*m-1);
	time(&end);

	printf("---- Dijkstra ----\n");
	printf("Execution time: %.6lf\n", difftime(begin,end));
	printf("Shortest path: %d\n\n", d);

	time(&begin);
	d = aStar(maze,0,n*m-1);
	time(&end);
	printf("---- A* ----\n");
	printf("Execution time: %.6lf\n", difftime(begin,end));
	printf("Shortest path: %d\n", d);

	return 0;
}