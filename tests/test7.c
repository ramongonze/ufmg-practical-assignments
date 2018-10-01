#include <stdlib.h>
#include <stdio.h>
#include "dccthread.h"

void texit(int dummy)
{
	dccthread_t *self = dccthread_self();
	printf("%s exiting\n", dccthread_name(self));
	dccthread_exit();
}

void test(int cnt)
{
	dccthread_t *self = dccthread_self();
	int i;
	for(i = 0; i < cnt; i++) {
		char name[16];
		sprintf(name, "t%d", i);
		printf("%s creating %s\n", dccthread_name(self), name);
		dccthread_t *t = dccthread_create(name, texit, 0);
		printf("%s yielding %s\n", dccthread_name(self), name);
		dccthread_yield();
		printf("%s waiting %s\n", dccthread_name(self), name);
		dccthread_wait(t);
		printf("%s continuing\n", dccthread_name(self));
	}
	dccthread_exit();
}

int main(int argc, char **argv)
{
	dccthread_init(test, 5);
}

