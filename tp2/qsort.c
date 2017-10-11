#include "qsort.h"

void Partition(int Lef, int Rig, int *i, int *j, Record *A){
	Record *pivot, w;
	*i = Lef;
	*j = Rig;

 	// Get the pivot by calculing the median among the first, the last and the central item of the array
	pivot = getPivot(A, Lef, Rig);

	do{
		while (isSmaller(&A[*i], pivot) == 1){
			(*i)++;
		}
		while (isSmaller(pivot, &A[*j]) == 1){
			(*j)--;
		}
		if (*i <= *j){
			copyRecord(&w, &A[*i]);
			copyRecord(&A[*i], &A[*j]);
			copyRecord(&A[*j], &w);
			(*i)++; (*j)--;
		}
	}while (*i <= *j);

	free(pivot);
}

void Sort(int Lef, int Rig, Record *A){
	int i, j;
	Partition(Lef, Rig, &i, &j, A);
	if (Lef < j) Sort(Lef, j, A);
	if (i < Rig) Sort(i, Rig, A);
}

void Quicksort(Record *A, int n){ // N = M/32
	Sort(0, n-1, A);
}

Record* getPivot(Record *A, int Lef, int Rig){
	Record *pivot = (Record *) malloc(sizeof(Record));
	int k = (Lef + Rig) / 2;

	if(isSmaller(&A[Lef], &A[Rig])){
		if(isSmaller(&A[Rig], &A[k])){
			copyRecord(pivot, &A[Rig]);
		}else if(isSmaller(&A[k], &A[Lef])){
			copyRecord(pivot, &A[Lef]);
		}else{
			copyRecord(pivot, &A[k]);
		}
	}else if(isSmaller(&A[Rig], &A[k])){
		if(isSmaller(&A[k], &A[Lef])){
			copyRecord(pivot, &A[k]);
		}else{
			copyRecord(pivot, &A[Lef]);
		}
	}else{
		copyRecord(pivot, &A[Rig]);
	}

	return pivot;
}
