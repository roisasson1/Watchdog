#ifndef __SDLL__H__
#define __SDLL__H__

#include <stddef.h>
#include "dll.h"

typedef int(*comperator_t)(void*, void*);
typedef struct list sdll_t;

typedef struct sdll_iterator
{
    dll_iterator_t iterator;
    #ifndef NDEBUG
    sdll_t* list;
    #endif
} sdll_iterator_t;

/*
    Description: Creates a sorted doubly linked list
    Args: A comparator function to compare elements
    Return Value: A pointer to the created sorted doubly linked list
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
sdll_t* SdllCreate(comperator_t comperator);

/*
    Description: Destroys a sorted doubly linked list and frees all allocated memory
    Args: A pointer to the sorted doubly linked list
    Return Value: None
    Time Complexity: O(n)
    Space Complexity: O(1)
*/
void SdllDestroy(sdll_t* s_list);

/*
    Description: Removes and returns the last element in the sorted doubly linked list
    Args: A pointer to the sorted doubly linked list
    Return Value: A pointer to the removed data
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
void* SdllPopBack(sdll_t* s_list);

/*
    Description: Removes and returns the first element in the sorted doubly linked list
    Args: A pointer to the sorted doubly linked list
    Return Value: A pointer to the removed data
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
void* SdllPopFront(sdll_t* s_list);

/*
    Description: Inserts an element into the sorted doubly linked list in its sorted position
    Args: A pointer to the sorted doubly linked list, and the data to insert
    Return Value: An iterator to the inserted element
    Time Complexity: O(n)
    Space Complexity: O(1)
*/
sdll_iterator_t SdllInsert(sdll_t* s_list, void* data);

/*
    Description: Removes an element from the sorted doubly linked list
    Args: An iterator to the element to remove
    Return Value: An iterator to the next element
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
sdll_iterator_t SdllRemove(sdll_iterator_t s_iterator);

/*
    Description: Finds an element in a range within the sorted doubly linked list, using Comperator function to compare
    Args: Iterators defining the range (from, to), the data to search for, and the sorted doubly linked list
    Return Value: An iterator to the found element, or to the end if not found
    Time Complexity: O(n)
    Space Complexity: O(1)
*/
sdll_iterator_t SdllFind(sdll_iterator_t from, sdll_iterator_t to, void* data, sdll_t* s_list);

/*
    Description: Finds an element in a range within the sorted doubly linked list, using IsMatch function to compare
    Args: Iterators defining the range (from, to), the data to match, and a match function
    Return Value: An iterator to the found element, or to the end if not found
    Time Complexity: O(n)
    Space Complexity: O(1)
*/
sdll_iterator_t SdllFindIf(sdll_iterator_t from, sdll_iterator_t to, void* data, int (*IsMatch)(void*, void*));

/*
    Description: Gets the number of elements in the sorted doubly linked list
    Args: A pointer to the sorted doubly linked list
    Return Value: The number of elements
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
size_t SdllSize(const sdll_t* s_list);

/*
    Description: Checks if the sorted doubly linked list is empty
    Args: A pointer to the sorted doubly linked list
    Return Value: 1 if empty, 0 otherwise
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
int SdllIsEmpty(const sdll_t* s_list);

/*
    Description: Gets the data stored at an iterator
    Args: An iterator to the element
    Return Value: A pointer to the data
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
void* SdllGetData(const sdll_iterator_t s_iterator);

/*
    Description: Gets the iterator to the next element in the sorted doubly linked list
    Args: An iterator to the current element
    Return Value: An iterator to the next element
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
sdll_iterator_t SdllGetNext(const sdll_iterator_t s_iterator);

/*
    Description: Gets the iterator to the previous element in the sorted doubly linked list
    Args: An iterator to the current element
    Return Value: An iterator to the previous element
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
sdll_iterator_t SdllGetPrev(const sdll_iterator_t s_iterator);

/*
    Description: Gets the iterator to the first element in the sorted doubly linked list
    Args: A pointer to the sorted doubly linked list
    Return Value: An iterator to the first element
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
sdll_iterator_t SdllBegin(const sdll_t* s_list);

/*
    Description: Gets the iterator to the end of the sorted doubly linked list (dummy node)
    Args: A pointer to the sorted doubly linked list
    Return Value: An iterator to the end
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
sdll_iterator_t SdllEnd(const sdll_t* s_list);

/*
    Description: Applies OperationFunc function to each element in a range of the sorted doubly linked list
    Args: Iterators defining the range (from, to), the operation function and additional arguments
    Return Value: SUCCESS if successful, OPERATION_FAIL otherwise
    Time Complexity: O(n)
    Space Complexity: O(1)
*/
status_t SdllForEach(sdll_iterator_t from, sdll_iterator_t to, int (*OperationFunc)(void*, void*), void* args);

/*
    Description: Compares two iterators for equality
    Args: Two iterators to compare
    Return Value: 1 if equal, 0 otherwise
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
int SdllIsEqual(sdll_iterator_t s_iterator1, sdll_iterator_t s_iterator2);

/*
    Description: Merges two sorted doubly linked lists into list1, and list2 will become an empty list
    Args: Pointers to the two sorted doubly linked lists
    Return Value: None
    Time Complexity: O(n + m)
    Space Complexity: O(1)
*/
void SdllMerge(sdll_t* s_list1, sdll_t* s_list2);

#endif /* end of header guard __SDLL__H__ */
