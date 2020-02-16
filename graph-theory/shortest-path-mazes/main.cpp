#ifndef _ALGS_
#include "algs.hpp"
#endif

#include <ctime>

int main(int argc, char *argv[]){
	srand(time(NULL)); // Used in function generateMaze()

	int n, m; // Maze size
	int d;
	clock_t begin, end;

	Graph maze;

	if(argc < 3){
		printf("There are few arguments!\n");
		return -1;
	}

	n = atoi(argv[1]);
	m = atoi(argv[2]);

	maze = generateMaze(n,m);
	
	// Uncomment the line above to generate a file which can print
	// the generated maze using the lib matplotlib from python 3.
	//printMaze(maze,n,m);

	begin = clock();
	d = dijkstra(maze,0,n*m-1);
	end = clock();

	printf("---- Dijkstra ----\n");
	printf("Execution time: %.8f\n", (float)(end - begin)/ CLOCKS_PER_SEC);
	printf("Shortest path: %d\n\n", d);

	begin = clock();
	d = aStar(maze,0,n*m-1);
	end = clock();

	printf("---- A* ----\n");
	printf("Execution time: %.8f\n", (float)(end - begin)/ CLOCKS_PER_SEC);
	printf("Shortest path: %d\n", d);

	return 0;
}