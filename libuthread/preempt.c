#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "preempt.h"
#include "uthread.h"
/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

void uthread_pause(int sig)
{
	// current thread in execution yields
	printf("Preempt yielding\n");
	uthread_yield();
}

void preempt_disable(void)
{
	/* TODO Phase 4 */
	// pushes alarm to no function
	signal(SIGVTALRM, NULL);
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
	// use the pause function
	signal(SIGVTALRM, uthread_pause);
}


void preempt_start(void)
{
	/* TODO Phase 4 */
	// enable preempt
	preempt_enable();

	// init itimer
	struct itimerval new;
	new.it_interval.tv_usec = 1.0 / HZ * 1000;
	new.it_interval.tv_sec = 0;
	new.it_value.tv_usec = 1.0 / HZ * 1000;
	new.it_value.tv_sec = 0;
	setitimer (ITIMER_VIRTUAL, &new, NULL);
	return;
}

