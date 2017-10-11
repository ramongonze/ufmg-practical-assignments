#include "tapes.h"

void Partition(int Lef, int Rig, int *i, int *j, Record *A);

void Sort(int Lef, int Rig, Record *A);

void Quicksort(Record *A, int n);

Record* getPivot(Record *A, int Lef, int Rig);
