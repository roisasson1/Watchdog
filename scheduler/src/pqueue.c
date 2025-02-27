/*
Author: Roi Sasson
Date: 21-12-2024
Reviewer: Yechiel Shirion
*/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "pqueue.h"

struct pqueue
{
    heap_t* list;
};

pqueue_t* PQCreate(pq_comperator_t comperator)
{
	pqueue_t* queue = (pqueue_t*)malloc(sizeof(pqueue_t));
	
	if (NULL == queue)
	{
		return NULL;
	}
	
	queue->list = HeapCreate(comperator);
	if (NULL == queue->list)
	{
		free(queue);
		return NULL;
	}
	
	return queue;
}

void PQDestroy(pqueue_t* queue)
{
	assert(NULL != queue);
	assert(NULL != queue->list);
	
	HeapDestroy(queue->list);
	free(queue);
	queue = NULL;
}

int PQEnqueue(pqueue_t* queue, void* data)
{
	assert(NULL != queue);
	assert(NULL != queue->list);

	return HeapPush(queue->list, data);
}

void* PQDequeue(pqueue_t* queue)
{
	void* popped_item = NULL;
	assert(NULL != queue);
	assert(NULL != queue->list);

	if (PQIsEmpty(queue))
    {
        return NULL;
    }

	popped_item = HeapPeek(queue->list);
	HeapPop(queue->list);
	return popped_item;
}

void* PQPeek(const pqueue_t* queue)
{
	assert(NULL != queue);
	assert(NULL != queue->list);
	
	return HeapPeek(queue->list);
}

int PQIsEmpty(const pqueue_t* queue)
{
	assert(NULL != queue);
	assert(NULL != queue->list);
	
	return HeapIsEmpty(queue->list);
}

size_t PQSize(const pqueue_t* queue)
{
	assert(NULL != queue);
	assert(NULL != queue->list);
	
	return HeapSize(queue->list);
}

void* PQErase(pqueue_t* queue, void* data, int (*IsMatch)(void*, void*))
{
	assert(NULL != queue);
	assert(NULL != queue->list);
	assert(NULL != data);

	return HeapRemove(queue->list, IsMatch, data);
}

void PQClear(pqueue_t* queue)
{
	assert(NULL != queue);
	
	while (!PQIsEmpty(queue))
	{
		PQDequeue(queue);
	}
}
