/*
    Version 1.1.0
*/

#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h>
#include "vector.h"

/*Description: This function compares between to values.
                Acts as a comperator.*/
typedef int (*heap_compare_func_t)(void*, void*);
/*Description: This function returns if a data is equals to the params.
                It compares each item in the heap (data) with the params.
                Case there's a match - 1 is returns, 0 otherwise.*/
typedef int (*heap_is_match_t)(void* data, void* params);
/*Description: type definition to the the Heap ds.*/
typedef struct heap heap_t; 

/*
    Description:        Creates the DS according to compare_func.
                        This function allocates and initiate the DS.
    Args:               compare_func - compare function.(refer heap_compare)
    Return value:       Pointer to the DS on success, NULL otherwise.
    Time complexity:    O(1).
    Space complexity:   O(1).
*/
heap_t* HeapCreate(heap_compare_func_t compare_func);

/*
    Description:        This function free all memory allocated by the DS.
    Args:               Heap - pointer to the DS.
    Return value:       None.
    Time complexity:    O(1).
    Space complexity:   O(1).
*/
void HeapDestroy(heap_t* heap);

/*
    Description:        Insert value to the heap ds.
                        This function stores the derefences value within data
                        in the DS. It places it according to the heap_compare.
    Args:               Heap - pointer to the ds.
                        Data - pointer to the data to be stored.
    Return value:       0 on Success, 1 otherwise.
    Time complexity:    O (log(n)).
    Space complexity:   O(1).
*/

int HeapPush(heap_t* heap, void* data); /* O (log(n)) */

/*
    Description:        Retrieves the highest value according to the heap_compare.
                        Case Heap is empty - undefinied behaviour.
    Args:               Heap - pointer to the ds.
    Return value:       Pointer to the highest value.
    Time complexity:    O(1).
    Space complexity:   O(1).
*/
void* HeapPeek(const heap_t* heap);

/*
    Description:        Removes the highest value according to the heap_compare.
                        Case Heap is empty - undefinied behaviour.
    Args:               Heap - pointer to the ds.
    Return value:       None.
    Time complexity:    O(log(n)).
    Space complexity:   O(1).
*/
void HeapPop(heap_t* heap);

/*
    Description:        This function removes a value according to param supplied.
                        (Refer heap_is_match)
                        is_match - function to compare between values.
                        params - a value used in is_match function, to seek
                        the desired value.
    Return value:       Pointer to the removed item on success, NULL otherwise.
    Time complexity:    O(n).
    Space complexity:   O(1).
*/
void* HeapRemove(heap_t* heap, heap_is_match_t is_match, void* params);
/*
    Description:        Returns if heap has no items within the Heap ds.
    Args:               Heap - pointer to the ds.
    Return value:       1 if true, 0 otherwise.
    Time complexity:    O(1).
    Space complexity:   O(1).
*/
int HeapIsEmpty(const heap_t* heap); /* O(1) */

/*
    Description:        Returns the amount of items within the Heap ds.
    Args:               Heap - pointer to the ds.
    Return value:       Amount of items within the ds.
    Time complexity:    O(1).
    Space complexity:   O(1).
*/
size_t HeapSize(const heap_t* heap); /* O(n) */

#endif /* __HEAP_H__*/