#include <stdlib.h>
#include <stdio.h>
#include "dccthread.h"

void test3b(int dummy)
{
	printf("entered %s with parameter %d\n", __func__, dummy);
}

void test(int dummy)
{
	printf("entered %s with parameter %d\n", __func__, dummy);
	dccthread_create("test3b", test3b, 0);
}

int main(int argc, char **argv)
{
	dccthread_init(test, 3);
}
