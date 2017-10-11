#include "algorithms.h"

Numbers *readNumbers(int N){
	int i, j, k;
	Numbers *options = (Numbers *) malloc(N * sizeof(Numbers));

	options->bars = (int *) malloc(N * sizeof(Numbers));
	options->houses = (int *) malloc(N * sizeof(Numbers));
	options->evenArray = (int *) malloc(N * sizeof(Numbers));
	options->oddArray = (int *) malloc(N * sizeof(Numbers));
	options->n = N;

	for(i = j = k = 0; i < N; i++){
		scanf("%d %d", &(options->bars[i]), &(options->houses[i]));
		if((options->bars[i]%2) == 0){
			options->evenArray[j] = options->bars[i];
			j++;
		}else{
			options->oddArray[k] = options->bars[i];
			k++;
		}
		if((options->houses[i]%2) == 0){
			options->evenArray[j] = options->houses[i];
			j++;
		}else{
			options->oddArray[k] = options->houses[i];
			k++;
		}
	}

	return options;
}

int greedy(Numbers *options){
	
	int i, j, max_flags, test_options;
	int *distances;
	int *solution;

	Quicksort(options->evenArray, options->n);
	Quicksort(options->oddArray, options->n);

	distances = countDistances(options);
	solution = (int *) malloc(options->n * sizeof(int));

	for(i = 0; i < options->n; i++){ solution[i] = 0; }

	max_flags = test_options = 0;
	for(i = 0; i < (options->n - test_options); i++){
		for(j = 0; j < options->n; j++){
			if(solution[j] == 0 && distances[j] == i){
				if(checkCandidate(options, solution, j) == 1){
					solution[j] = 1;
					max_flags++;
					test_options++;
				}
			}
		}
	}// Main loop

	free(solution);
	free(distances);
	return max_flags;
}

int checkCandidate(Numbers *options, int *solution, int candidate){

	int i, test = 2;

	for(i = 0; i < options->n; i++){ // Check all candidates in the solution set
		if(solution[i] == 1){
			if(options->evenArray[candidate] > options->evenArray[i]){
				if( findCorrespondent(options, options->evenArray[candidate]) > findCorrespondent(options, options->evenArray[i]) ){
					test =  1; // Valid candidate
				}else{
					return 0; // Not a valid candidate
				}
			}else{
				if( findCorrespondent(options, options->evenArray[candidate]) < findCorrespondent(options, options->evenArray[i]) ){
					test = 1; // Valid candidate
				}else{
					return 0; // Not a valid candidate
				}
			}
		}
	}

	if(test == 2){ return 1; } // First candidate that is beeing analyzed.
	return test;

}

int* countDistances(Numbers *options){

	int i, j, *distances, aux_even, aux_odd, even, odd;

	distances = (int *) malloc(options->n * sizeof(int));

	for(i = 0; i < options->n; i++){
		even = options->evenArray[i];
		odd = findCorrespondent(options, even);

		for(j = 0; j < options->n; j++){
			if(even == options->evenArray[j]){ aux_even = j; }
			if(odd == options->oddArray[j]){ aux_odd = j; }
		}

		distances[i] = abs(aux_odd - aux_even);
	}

	return distances;
}

int dynamic(Numbers *options){

	int i, j, aux, max_flags;
	int *LIS;

	LIS = (int *) malloc(options->n * sizeof(int));
	
	Quicksort(options->evenArray, options->n);
	
	for(i = 0; i < options->n; i++){
		options->oddArray[i] = findCorrespondent(options, options->evenArray[i]);
	}

	LIS[0] = 1;
	max_flags = 0;
	for(i = 0; i < options->n; i++){
		aux = 0;
		for(j = 0; j < (i+1); j++){
			if(options->oddArray[i] > options->oddArray[j] && LIS[j] > aux){
				aux = LIS[j];
			} 
		}
		if(i) LIS[i] = aux + 1;
		if(LIS[i] > max_flags){
			max_flags = LIS[i];
		}
	}

	free(LIS);
	return max_flags;
}

int bruteForce(Numbers *options){

	int *possibilities, max_flags, i, j, k, number_of_flags, odd_i, even_i, aux;
	long long all_possibilities;

	possibilities = (int *) malloc(options->n * sizeof(int));

	Quicksort(options->evenArray, options->n);
	Quicksort(options->oddArray, options->n);

	for(i = 0, all_possibilities = 1; i < options->n; i++){ all_possibilities *= 2; }

	max_flags = 0;
	for(i = 0; i < all_possibilities; i++){
		aux = 0;
		for(j = options->n - 1, k = i; j >= 0; j--){
			possibilities[j] = (k - ((k/2)*2));
			k /= 2;
			if(possibilities[j] == 1){ aux++; }
		}

		if(aux > max_flags){
			j = odd_i = number_of_flags = 0;
			while(j < options->n){
				while(j < options->n && possibilities[j] == 0){ j++; }

				if(j < options->n){
					even_i = options->evenArray[j];
					aux = findCorrespondent(options, even_i);
					
					if(aux > odd_i){
						odd_i = aux;
						number_of_flags++;
						j++;
					}else{ // Two lines has crossed
						break;
					}
				}
			}
			
			if(number_of_flags > max_flags){
				max_flags = number_of_flags;
			}
		}	
	}
	
	free(possibilities);
	return max_flags;
}

int findCorrespondent(Numbers *options, int A){

	int i;

	for(i = 0; i < options->n; i++){
		if(options->bars[i] == A){
			return options->houses[i];
		}
		if(options->houses[i] == A){
			return options->bars[i];
		}
	}

	return -1;
}

void destroyNumbers(Numbers *options){

	free(options->bars);
	free(options->houses);
	free(options->evenArray);
	free(options->oddArray);
	free(options);
}
