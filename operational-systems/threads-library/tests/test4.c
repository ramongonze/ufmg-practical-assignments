#include <stdlib.h>
#include <stdio.h>
#include "dccthread.h"

void test(int dummy)
{
	char name[16];
	sprintf(name, "test%d", dummy);
	printf("parent of %s with parameter %d\n", name, dummy);
	if(dummy == 1) return;
	dccthread_create(name, test, dummy-1);
}

int main(int argc, char **argv)
{
	dccthread_init(test, 4);
}
