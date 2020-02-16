#include <stdlib.h>
#include <stdio.h>
#include "dccthread.h"

void test1(int cnt)
{
	dccthread_t *self = dccthread_self();
	while(cnt > 0) {
		printf("%s %d\n", dccthread_name(self), cnt);
		cnt--;
	}
}

int main(int argc, char **argv)
{
	dccthread_init(test1, 5);
}

