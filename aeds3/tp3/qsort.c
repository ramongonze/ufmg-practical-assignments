#include "qsort.h"

void Partition(int Lef, int Rig, int *i, int *j, int *A){
	int pivot, w;
	*i = Lef;
	*j = Rig;

	pivot = getPivot(A, Lef, Rig);

	do{
		while (pivot > A[*i]){
			(*i)++;
		}
		while (pivot < A[*j]){
			(*j)--;
		}
		if (*i <= *j){
			w = A[*i];
			A[*i] = A[*j];
			A[*j] = w;
			(*i)++; (*j)--;
		}
	}while (*i <= *j);

}

void Sort(int Lef, int Rig, int *A){
	int i, j;
	Partition(Lef, Rig, &i, &j, A);
	if (Lef < j) Sort(Lef, j, A);
	if (i < Rig) Sort(i, Rig, A);
}

void Quicksort(int *A, int n){ // N = M/32
	Sort(0, n-1, A);
}

int getPivot(int *A, int Lef, int Rig){
	int pivot;
	int k = (Lef + Rig) / 2;

	if(A[Lef] <= A[Rig]){
		if(A[Rig] <= A[k]){
			pivot = A[Rig];
		}else if(A[k] <= A[Lef]){
			pivot = A[Lef];
		}else{
			pivot = A[k];
		}
	}else if(A[Rig] <= A[k]){
		if(A[k] <= A[Lef]){
			pivot= A[k];
		}else{
			pivot = A[Lef];
		}
	}else{
		pivot = A[Rig];
	}

	return pivot;
}
