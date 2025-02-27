#ifndef __DLL_H__
#define __DLL_H__

typedef struct node dll_node_t;
typedef struct list dll_t;
typedef dll_node_t* dll_iterator_t;

typedef enum
{
    OPERATION_FAIL = -1,
    SUCCESS
} status_t;

/*
    Description: Creates a new doubly linked list
    Args: None
    Return Value: A pointer to the newly created list, or NULL if memory allocation fails
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
dll_t* DllCreate(void);

/*
    Description: Destroys the list and frees all allocated memory
    Args: A pointer to the list to destroy
    Return Value: None
    Time Complexity: O(n)
    Space Complexity: O(1)
*/
void DllDestroy(dll_t* list);

/*
    Description: Inserts a new element with the given data at the beginning of the list
    Args: A pointer to the list, A pointer to the data to insert
    Return Value: An iterator to the new element, or NULL on failure
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
dll_iterator_t DllPushFront(dll_t* list, void* data);

/*
    Description: Inserts a new element with the given data at the end of the list
    Args: A pointer to the list, A pointer to the data to insert
    Return Value: An iterator to the new element, or NULL on failure
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
dll_iterator_t DllPushBack(dll_t* list, void* data);

/*
    Description: Removes and retrieves the last element in the list
    Args: A pointer to the list
    Return Value: A pointer to the data stored in the removed element, or NULL if the list is empty
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
void* DllPopBack(dll_t* list);

/*
    Description: Removes and retrieves the first element in the list
    Args: A pointer to the list
    Return Value: A pointer to the data stored in the removed element, or NULL if the list is empty
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
void* DllPopFront(dll_t* list);

/*
    Description: Inserts a new element with the given data before the specified iterator
    Args: A pointer to the list, An iterator to the position to insert before, A pointer to the data to insert
    Return Value: An iterator to the new element, or NULL on failure
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
dll_iterator_t DllInsert(dll_t* list, dll_iterator_t iterator, void* data);

/*
    Description: Removes the element pointed to by the given iterator from the list
    		 User can't remove last iterator
    Args: An iterator pointing to the element to remove
    Return Value: An iterator to the next element after the removed one
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
dll_iterator_t DllRemove(dll_iterator_t iterator);

/*
    Description: Finds an element in a range within the sorted doubly linked list, using IsMatch function to compare
    Args: Iterators defining the range (from, to), the data to match, and a match function
    Return Value: An iterator to the found element, or to if no match is found
    Time Complexity: O(n)
    Space Complexity: O(1)
*/
dll_iterator_t DllFind(dll_iterator_t from, dll_iterator_t to, void* data, int (Is_Match)(void*, void*));

/*
    Description: Finds all elements in the range [from, to) that match the given data and stores them in the output list
    Args: 
        out_params: A pointer to the list where matched elements will be stored,
         I	    iterators defining the range (from, to), the data to match, and a match function
    Return Value: SUCCESS if all matches were stored successfully, OPERATION_FAIL otherwise
    Time Complexity: O(n)
    Space Complexity: O(n) for the output list
*/
status_t DllMultiFind(dll_t* out_params, dll_iterator_t from, dll_iterator_t to, void* data, int (*Is_Match)(void*, void*));

/*
    Description: Returns the number of elements in the list
    Args: A pointer to the list
    Return Value: The number of elements in the list
    Time Complexity: O(n)
    Space Complexity: O(1)
*/
size_t DllSize(const dll_t* list);

/*
    Description: Checks if the list is empty
    Args: A pointer to the list
    Return Value: 1 if empty, 0 otherwise
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
int DllIsEmpty(const dll_t* list);

/*
    Description: Retrieves the data stored in the element pointed to by the given iterator
    Args: An iterator pointing to the element
    Return Value: A pointer to the data stored in the element
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
void* DllGetData(const dll_iterator_t iterator);

/*
    Description: Sets the data in the element pointed to by the given iterator
    Args: An iterator pointing to the element, A pointer to the new data
    Return Value: None
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
void DllSetData(dll_iterator_t iterator, void* data);

/*
    Description: Gets an iterator to the next element in the list
    Args: An iterator pointing to the current element
    Return Value: An iterator to the next element, or NULL if the current element is the last
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
dll_iterator_t DllGetNext(const dll_iterator_t iterator);

/*
    Description: Gets an iterator to the previous element in the list
    Args: An iterator pointing to the current element
    Return Value: An iterator to the previous element, or NULL if the current element is the first
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
dll_iterator_t DllGetPrev(const dll_iterator_t iterator);

/*
    Description: Gets an iterator to the first element in the list
    Args: A pointer to the list
    Return Value: An iterator to the first element, or NULL if the list is empty
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
dll_iterator_t DllBegin(const dll_t* list);

/*
    Description: Gets an iterator to the dummy (end) element in the list
    Args: A pointer to the list
    Return Value: An iterator to the dummy (end) element
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
dll_iterator_t DllEnd(const dll_t* list);

/*
    Description: Applies OperationFunc function to each element in a range of the sorted doubly linked list
    Args: Iterators defining the range (from, to), the operation function and additional arguments
    Return Value: SUCCESS if all operations succeeded, OPERATION_FAIL otherwise
    Time Complexity: O(n)
    Space Complexity: O(1)
*/
status_t DllForEach(dll_iterator_t from, dll_iterator_t to, int (*operation_func)(void*, void*), void* args);

/*
    Description: Checks if two iterators point to the same element
    Args: Two iterators to compare
    Return Value: 1 iterators are equal, 0 otherwise
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
int DllIsEqual(dll_iterator_t iterator1, dll_iterator_t iterator2);

/*
    Description: Splices a range of elements from source list into target list before a specified iterator,
    		 After the splice, source list will have the remaining elements
    Args: An iterator in the target list before which elements will be inserted,
	  Iterators defining the range (from, to) in source list
    Return Value: None
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
void DllSplice(dll_iterator_t iterator, dll_iterator_t from, dll_iterator_t to);

#endif /* end of header guard __DLL_H__ */

