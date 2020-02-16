#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dccthread.h"

void tsleep(int seconds) {
	dccthread_t *self = dccthread_self();
	struct timespec ts;
	ts.tv_sec = seconds;
	ts.tv_nsec = 0;
	dccthread_sleep(ts);
	printf("thread %s woke up and exiting\n", dccthread_name(self));
	dccthread_exit();
}

void test(int cnt) {
	int i;
	dccthread_t **threads = calloc(cnt, sizeof(*threads));
	assert(threads);
	for(i = 0; i < cnt; i++) {
		char name[16];
		sprintf(name, "sleep%d", i);
		threads[i] = dccthread_create(name, tsleep, i+1);
	}
	for(i = 0; i < cnt*2; i++) {
		dccthread_yield();
		printf("thread main running\n");
	}
	for(i = 0; i < cnt; i++) {
		dccthread_wait(threads[i]);
	}
	printf("main thread exiting\n");
	dccthread_exit();
}

int main(int argc, char **argv)
{
	dccthread_init(test, 5);
}

