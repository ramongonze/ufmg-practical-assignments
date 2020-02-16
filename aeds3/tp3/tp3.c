#include "qsort.h"
#include "algorithms.h"

int main(){

	char algorithm;
	int max_flags, N;
	Numbers *options;

	scanf("%c", &algorithm);
	scanf("%d", &N);

	options = readNumbers(N);

	switch(algorithm){
		case 'g':
			max_flags = greedy(options);
			break;
		case 'd':
			max_flags = dynamic(options);
			break;
		case 'b':
			max_flags = bruteForce(options);
			break;
		default:
			printf("Error! Invalid algorithm.\n");
			return 0;
	}

	printf("%d\n", max_flags);

	destroyNumbers(options);
	
	return 0;
}