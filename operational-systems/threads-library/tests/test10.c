#include <stdlib.h>
#include <stdio.h>
#include "dccthread.h"

int global = 0;

void tloop(int cnt) {
	int i;
	for(i = 0; i < cnt; i++) {
		dccthread_yield();
	}
	dccthread_exit();
}

void test(int cnt) {
	dccthread_t *t = dccthread_create("aux", tloop, cnt);
	int i;
	for(i = 0; i < cnt; i++) {
		dccthread_yield();
	}
	printf("global counter is 0x%x\n", global);
	dccthread_wait(t);
	dccthread_exit();
}

int main(int argc, char **argv)
{
	dccthread_init(test, 1000000);
}

