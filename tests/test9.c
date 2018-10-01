#include <stdlib.h>
#include <stdio.h>
#include "dccthread.h"

int global = 0;

void tloop(int cnt) {
	int i;
	for(i = 0; i < cnt; i++) {
		if(global & 0x1) { global |= 0x2; }
		if(global & 0x4) { global |= 0x8; }
	}
	dccthread_exit();
}

void test(int cnt) {
	dccthread_t *t = dccthread_create("aux", tloop, cnt);
	global |= 0x1;
	int i;
	for(i = 0; i < cnt; i++) {
		if(global & 0x2) { global |= 0x4; }
	}
	printf("global counter is 0x%x\n", global);
	dccthread_wait(t);
	dccthread_exit();
}

int main(int argc, char **argv)
{
	dccthread_init(test, 100000000);
}

