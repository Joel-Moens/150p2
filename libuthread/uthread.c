#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ucontext.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"

#define READY 0
#define RUNNING 1
#define BLOCKED 2
#define ZOMBIE 3

/* TODO Phase 2 */
int tidCount = 0;
queue_t tQueue;

typedef struct thread_block {
	uthread_t id;
	uthread_ctx_t *context;
	void * sp;
	int state;
	int ret;
	struct thread_block* collect;
	struct thread_block* parent;
} thread_block;

thread_block* tmain;

// mallocs a new thread control block
thread_block* uthread_malloc() {

 	thread_block* tbc = (thread_block *)malloc(sizeof(thread_block));
 	tbc->collect = NULL;
 	tbc->state = READY;
 	tbc->parent = tmain;
	tbc->id = tidCount++;
	tbc->sp = uthread_ctx_alloc_stack();
	tbc->context = (uthread_ctx_t*) malloc(sizeof(uthread_ctx_t));
	return tbc;
}

// collects the thread
void uthread_free(thread_block* tbc) {
	printf("Collecting: %d\n", tbc->id);
	uthread_ctx_destroy_stack(tbc->sp);
	free(tbc->context);
	return;
}

// initializes the main thread -- context already exists
void uthread_init()
{
	tQueue = queue_create();
	tmain = uthread_malloc();
	tmain->state = RUNNING;
	// gets the context of main
	getcontext(tmain->context);
	preempt_start();
}

// function for debugging 
static int print_items(queue_t queue, void *data, void *arg)
{
	thread_block * temp = (thread_block*) arg;

    printf("Queue print tid||state: %d || %d\n", temp->id, temp->state);

    return 0;
}

// finds and returns the next ready thread 
// mangages threads
thread_block* uthread_ready(void** tbc) {
	// search for ready thread
	for (int i = 0; i < queue_length(tQueue); i++) {
		queue_dequeue(tQueue, tbc);
		// tbc is ready

		thread_block* tb = (thread_block*)(*tbc);


		if (tb->state == READY) 
		{
			printf("Pick: %d\n", tb->id);
			return (thread_block*)(*tbc);
		} // found a ready htread
		if (tb->collect != NULL){
			((thread_block*)(*tbc))->state = READY;
		} // tb not ready enqueue and repeat
 	
		queue_enqueue(tQueue, (thread_block*)(*tbc));
	}

	return NULL;
	
}

// yields to ready threads
void uthread_yield(void)
{
	/* TODO Phase 2 */
	preempt_disable();
	printf("Yield: %d\n", tmain->id);
	int len = queue_length(tQueue);
	// there are threads to yield
	if (len != 0) {
		void** tbc = (void**) malloc(sizeof(thread_block*));
		if (tmain->state == RUNNING) {
			tmain->state = READY;
		} // stage current thread as ready

		// debug
		queue_iterate(tQueue, print_items, (void*)0, NULL);

		// get ready thread
		thread_block* ready = uthread_ready(tbc);
		
		if (ready == NULL) {
			perror("No ready threads");
			return;
		} // no ready threads
		
		// enque running thread
		uthread_ctx_t* prev = tmain->context;
		queue_enqueue(tQueue, tmain);


		// change current execution to the first item in the queue
		tmain = ready;
		uthread_ctx_t* next = tmain->context;
		tmain->state = RUNNING;
		
		// swaps context
		preempt_enable();
		uthread_ctx_switch(prev, next);

	} else {
		perror("yield\n");
	} // there are no threadss
}


// returns the running threads id
uthread_t uthread_self(void)
{
	/* TODO Phase 2 */
	return tmain->id;
}

// creates and starts new threads (twice if main has not been init)
int uthread_create(uthread_func_t fu nc, void *arg)
{
	/* TODO Phase 2 */
	preempt_disable();
	if(tidCount == 0) {
		uthread_init();
	} // create main thread and get its context

	// Make a thread block with a context pointer
	thread_block* tbc = uthread_malloc();
	// initializes the context
	printf("Creating thread: %d\n", tbc->id);
	uthread_ctx_init(tbc->context, tbc->sp, func, arg);
	queue_enqueue(tQueue, tbc);
	preempt_enable();
	return tbc->id;
}

// thread function has ended and retruned rtval
void uthread_exit(int retval)
{
	/* TODO Phase 2 */
	preempt_disable();
	// exits current thread
	printf("Exiting: %d\n", tmain->id);
	// end process lifetime
	tmain->state = ZOMBIE;
	// signify that it is now collectable
	tmain->ret = retval;
	//if (tmain->parent->state != ZOMBIE) {
	printf("PID: %d\n", tmain->parent->id);
	tmain->parent->collect = tmain;
		// add to collect for parent
	// } // tell parent it is collectable
	
	// yield to other threads / wait for collection
	uthread_yield();
	
}

// 
int uthread_join(uthread_t tid, int *retval)
{
	preempt_disable();
	tmain->state = BLOCKED;
	/* TODO Phase 2 */
	printf("Join Target: %d\n", tid);

	thread_block* collect = tmain->collect;
	// void** data = malloc(sizeof(void*));

	// yield to gather new thread
	if (!collect || collect->id != tid) 
		uthread_yield();
	collect = tmain->collect;
	preempt_disable();
	printf("Found TID: %d\n", collect->id);
	// collect value if we want it
	if (retval)
		*retval = collect->ret;
	queue_iterate(tQueue, print_items, (void*)0, NULL);
	uthread_free((tmain->collect));
	queue_delete(tQueue, (tmain->collect));
	// tmain->collect = NULL;
	free((tmain->collect));
	preempt_enable();

	return 0;
	/* TODO Phase 3 */
}