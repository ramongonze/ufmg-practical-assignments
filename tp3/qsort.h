#include <stdio.h>
#include <stdlib.h>

/* This is the Quicksort algorithm, for integer numbers */

// The function below slip the array in two, putting smallers and highers elements.
// in different parts of the array, according the selected pivot.
void Partition(int Lef, int Rig, int *i, int *j, int *A);

// The function below calls the function 'Partition' and sort the new two parts created.
void Sort(int Lef, int Rig, int *A);

// The function receives a integer array, and orders it.
void Quicksort(int *A, int n);

// The function below take 3 numbers of the array, and select the median among them. 
// This median will be the pivot.
int getPivot(int *A, int Lef, int Rig);
