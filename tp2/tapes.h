#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_STR_SIZE 255
#define MAX_WORD_SIZE 20
#define INT_SIZE 4
#define MAX_RECORD_SIZE 32
#define BUFFER_SIZE 259

typedef struct Record{
	char word[MAX_WORD_SIZE];
	int d;
	int f;
	int p;
}Record;

void Quicksort(Record *A, int n);

int isSmaller(Record *A, Record *B);

void copyRecord(Record *A, Record *B);

void buildUnsortedIndex(int D, char E[], char S[]);

int buildTapes(char S[], int tapes_amount);

void intercalate(int tapes_amount, char S[]);

void putFrequency(char S[]);
