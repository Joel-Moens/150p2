#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "queue.h"

int main()
{
	printf("Trying to make a queue \n");
	int i;
	void * ptr;
	ptr = (void*) malloc (sizeof(void));
	ptr = &i;
	i = 100;
	void ** ret;
	ret = (void **) malloc (sizeof(void));
	queue_t testq;
	testq = queue_create();
	queue_enqueue(testq, ptr);
	printf("Queue size is %d and value is %i ptr address is %p \n", queue_length(testq), *(int*)ptr, ptr);
	queue_dequeue(testq, ret);
	printf("Queue size is %d, and dequeue returned %i, ret address is %p \n", queue_length(testq), *(int*)*ret, *ret);

	
	if(queue_destroy(testq) == 0)
	{
		printf("Success! \n");
		testq = NULL;
	}
	queue_enqueue(testq, ptr);
	queue_dequeue(testq, ret);
	if(queue_destroy(testq) == 0)
	{
		printf("Success! \n");
	}

	

	return 0;
}

