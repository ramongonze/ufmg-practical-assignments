#include "dlist.h"
#include "dccthread.h"

struct dlist *threads_list; // List of threads
ucontext_t manager;

// Variables for timer
timer_t timer;
struct sigevent timer_event;
struct sigaction timer_action, old_timer_action;
struct itimerspec time_in_CPU;

// Mask used to avoid race condition
sigset_t mask;

// A pointer for the curruent thread
dccthread_t *current_thread;

/* Check if the thread target is in the thread's list. Return != 0 on success 
 * or 0 otherwise. */
int find_thread(dccthread_t *target){
	if(dlist_empty(threads_list)) return 0;

	struct dnode *curr = threads_list->head;
	while((dccthread_t *)curr->data != target && curr != NULL) curr = curr->next;

	return curr != NULL;
}

static void stop_thread(int sig){
	dccthread_yield();
}

void timer_init(){
	timer_action.sa_flags = 0;
	timer_action.sa_handler = stop_thread;
	sigaction(SIGALRM, &timer_action, NULL);

	timer_event.sigev_notify = SIGEV_SIGNAL;
	timer_event.sigev_signo = SIGALRM;
	timer_event.sigev_value.sival_ptr = &timer;
	timer_create(CLOCK_PROCESS_CPUTIME_ID, &timer_event, &timer);

	time_in_CPU.it_value.tv_sec = time_in_CPU.it_interval.tv_sec = 0;
	time_in_CPU.it_value.tv_nsec = time_in_CPU.it_interval.tv_nsec = 10000000;
	timer_settime(timer, 0, &time_in_CPU, NULL);
}

void dccthread_init(void (*main)(int), int param){
	threads_list = dlist_create();

	timer_init();

	getcontext(&manager);

	// Create the mask for signals
	sigemptyset(&mask);
	sigaddset(&mask, SIGALRM);
	//sigprocmask(SIG_SETMASK, &mask, NULL);
	
	dccthread_create("main", main, param);
	while(!dlist_empty(threads_list)){
		//sigprocmask(SIG_BLOCK, &mask, NULL);
		current_thread = (dccthread_t *) dlist_pop_left(threads_list);

		// Check if the thread is waiting for another one
		if(current_thread->waiting != NULL){
			if(find_thread(current_thread->waiting)){
				dlist_push_right(threads_list, current_thread);
				continue;
			}else{
				current_thread->waiting = NULL;
			}
		}
		//sigprocmask(SIG_UNBLOCK, &mask, NULL);
		swapcontext(&manager,&(current_thread->context));
	}

	exit(EXIT_SUCCESS);
}

dccthread_t *dccthread_create(const char *name, void (*func)(int), int param){
	//sigprocmask(SIG_BLOCK, &mask, NULL);

	dccthread_t *new = (dccthread_t *) malloc(sizeof(dccthread_t));

	new->name = name;

	getcontext(&(new->context));
	new->context.uc_link = &manager;

	// Initialize the process stack
	new->context.uc_stack.ss_sp = (void*) malloc(SIGSTKSZ);
	new->context.uc_stack.ss_size = SIGSTKSZ;
	
	new->waiting = NULL;

	makecontext(&(new->context), (void (*) (void)) func, 1, param);

	dlist_push_right(threads_list, new);

	//sigprocmask(SIG_UNBLOCK, &mask, NULL);
	return new;	
}

/* `dccthread_yield` will yield the CPU (from the current thread to
 * another). */
void dccthread_yield(void){
	//sigprocmask(SIG_BLOCK, &mask, NULL);
	dlist_push_right(threads_list, current_thread);	
	swapcontext(&(current_thread->context), &manager);
	//sigprocmask(SIG_UNBLOCK, &mask, NULL);
}

/* `dccthread_exit` terminates the current thread, freeing all
 * associated resources. */
void dccthread_exit(void){
	//sigprocmask(SIG_BLOCK, &mask, NULL);
	free(current_thread);
	setcontext(&manager);
	//sigprocmask(SIG_UNBLOCK, &mask, NULL);
}

/* `dccthread_wait` blocks the current thread until thread `tid`
 * terminates. */
void dccthread_wait(dccthread_t *tid){
	//sigprocmask(SIG_BLOCK, &mask, NULL);
	if(!find_thread(tid)) return;
	
	current_thread->waiting = tid;
	dlist_push_right(threads_list, current_thread);
	swapcontext(&(current_thread->context), &manager);
	//sigprocmask(SIG_UNBLOCK, &mask, NULL);
}

/* `dccthread_sleep` stops the current thread for the time period
 * specified in `ts`. */
//void dccthread_sleep(struct timespec ts);

/* `dccthread_self` returns the current thread's handle. */
dccthread_t * dccthread_self(void){
	return current_thread;
}

/* `dccthread_name` returns a pointer to the string containing the
 * name of thread `tid`.  the returned string is owned and managed
 * by the library. */
const char * dccthread_name(dccthread_t *tid){
	return tid->name;
}