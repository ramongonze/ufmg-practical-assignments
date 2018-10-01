#include <stdlib.h>
#include <stdio.h>
#include "dccthread.h"

void test(int dummy)
{
	printf("entered %s with parameter %d\n", __func__, dummy);
}

int main(int argc, char **argv)
{
	dccthread_init(test, 2);
}
