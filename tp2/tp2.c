#include "tapes.h"

int main(){
	int D, M, tapes_amount;
	char E[MAX_STR_SIZE], S[MAX_STR_SIZE];

	scanf("%d %d %s %s", &D, &M, E, S);

	buildUnsortedIndex(D, E, S);

	tapes_amount = M/MAX_RECORD_SIZE;

	tapes_amount = buildTapes(S, tapes_amount);

	intercalate(tapes_amount, S);

	putFrequency(S);

	return 0;
}
