#ifndef __PQUEUE_H__
#define __PQUEUE_H__

#include <stddef.h>
#include "heap.h"

typedef struct pqueue pqueue_t;
typedef int(*pq_comperator_t)(void*, void*);

/*
    Description: Creates a priority queue
    Args: A comparator function to prioritize elements
    Return Value: A pointer to the created priority queue
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
pqueue_t* PQCreate(pq_comperator_t comperator);

/*
    Description: Destroys a priority queue and frees all allocated memory
    Args: A pointer to the queue
    Return Value: None
    Time Complexity: O(n)
    Space Complexity: O(1)
*/
void PQDestroy(pqueue_t* queue);

/*
    Description: Inserts an element into the priority queue based on its priority
    Args: A pointer to the queue, and the data to insert
    Return Value: SUCCESS if the operation is successful, MEMORY_ALLOCATION_FAIL otherwise
    Time Complexity: O(n) (due to sorted insertion)
    Space Complexity: O(1)
*/
int PQEnqueue(pqueue_t* queue, void *data);

/*
    Description: Removes the highest-priority element from the priority queue
    Args: A pointer to the queue
    Return Value: Data of the highest-priority element
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
void* PQDequeue(pqueue_t* queue);

/*
    Description: Retrieves the highest-priority element in the priority queue without removing it
    Args: A pointer to the  queue
    Return Value: A pointer to the data of the highest-priority element
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
void* PQPeek(const pqueue_t* queue);

/*
    Description: Checks if the priority queue is empty
    Args: A pointer to the queue
    Return Value: 1 if empty, 0 otherwise
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
int PQIsEmpty(const pqueue_t* queue);

/*
    Description: Gets the number of elements in the priority queue
    Args: A pointer to the queue
    Return Value: The number of elements
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
size_t PQSize(const pqueue_t* queue);

/*
    Description: Removes the first occurrence of an element with the given data from the priority queue
    		 if not found, do nothing
    Args: A pointer to the priority queue, and a pointer to the data to remove
    Return Value: None
    Time Complexity: O(n)
    Space Complexity: O(1)
*/
void* PQErase(pqueue_t* queue, void* data, int (*IsMatch)(void*, void*));

/*
    Description: Clears all elements from the priority queue
    Args: A pointer to the priority queue
    Return Value: None
    Time Complexity: O(n)
    Space Complexity: O(1)
*/
void PQClear(pqueue_t* queue);

#endif /* end of header guard __PQUEUE_H__ */
