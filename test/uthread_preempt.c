/*
 * Thread creation and yielding test
 *
 * Tests the creation of multiples threads and the fact that a parent thread
 * should get returned to before its child is executed. The way the printing,
 * thread creation and yielding is done, the program should output:
 *
 * 
 * thread2
 * thread3
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

int thread3(void* arg)
{
	uthread_yield();
	printf("thread%d\n", uthread_self());
	while (1) {
		// while loop should be interrupt
	}
	return 0;
}

int thread2(void* arg)
{
	uthread_create(thread3, NULL);
	//uthread_yield();
	printf("thread%d\n", uthread_self());
	return 2;
}

int thread1(void* arg)
{
	while (1) {
		// while loop should be interrupt
	}
	printf("thread%d\n", uthread_self());
	return 0;
}

int main(void)
{
	int b;
	uthread_create(thread1, NULL);
	uthread_join(uthread_create(thread2, NULL), &b);
	uthread_yield();
	printf("%d\n", b);
	return 0;
}
