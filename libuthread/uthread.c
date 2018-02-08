#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"

/* TODO Phase 2 */
int tidCount = 0;
typedef struct thread_block {
	uthread_t id;
	uthrad_ctx_t *context;
	void * sp;
	int state;
}thread_block;

queue_t tQueue;

void uthread_yield(void)
{
	/* TODO Phase 2 */
}

uthread_t uthread_self(void)
{
	/* TODO Phase 2 */
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	if (tidCount == 0)
	{


	}
	thread_block * tblock;
	tblock = malloc(sizeof(tblock));
	tblock-> id = tidCount;
	tblock->sp = uthread_ctx_alloc_stack();
	tblock->context = malloc(sizeof(uthread_ctx_t));
	//Make a thread block with a context pointer

	//Call uthread_ctx_init with the new tblock and given arguments
	uthread_ctx_init(tblock->context, tblock->sp, func, arg);
}

void uthread_exit(int retval)
{
	/* TODO Phase 2 */
}

int uthread_join(uthread_t tid, int *retval)
{
	/* TODO Phase 2 */
	/* TODO Phase 3 */
}

void init(void)
{
	//Allocate the queue
	tQueue = queue_create();
	//Init the main thread
	thread_block * tmain;
}

