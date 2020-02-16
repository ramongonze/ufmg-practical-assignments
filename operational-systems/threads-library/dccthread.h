#ifndef __DCCTHREAD_HEADER__
#define __DCCTHREAD_HEADER__

#include <time.h>

typedef struct dccthread dccthread_t;

#define DCCTHREAD_MAX_NAME_SIZE 256
#define THREAD_STACK_SIZE (1<<16)

/* `dccthread_init` initializes any state necessary for the
 * threadling library and starts running `func`.  this function
 * never returns. */
void dccthread_init(void (*func)(int), int param) __attribute__((noreturn));

/* on success, `dccthread_create` allocates and returns a thread
 * handle.  returns `NULL` on failure.  the new thread will execute
 * function `func` with parameter `param`.  `name` will be used to
 * identify the new thread. */
dccthread_t * dccthread_create(const char *name,
		void (*func)(int ), int param);

/* `dccthread_yield` will yield the CPU (from the current thread to
 * another). */
void dccthread_yield(void);

/* `dccthread_exit` terminates the current thread, freeing all
 * associated resources. */
void dccthread_exit(void);

/* `dccthread_wait` blocks the current thread until thread `tid`
 * terminates. */
void dccthread_wait(dccthread_t *tid);

/* `dccthread_sleep` stops the current thread for the time period
 * specified in `ts`. */
void dccthread_sleep(struct timespec ts);

/* `dccthread_self` returns the current thread's handle. */
dccthread_t * dccthread_self(void);

/* `dccthread_name` returns a pointer to the string containing the
 * name of thread `tid`.  the returned string is owned and managed
 * by the library. */
const char * dccthread_name(dccthread_t *tid);

#endif
