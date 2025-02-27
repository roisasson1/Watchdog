#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h> /* size_t */

typedef struct vector vector_t;

typedef enum
{
	ALLOCATION_FAIL = -2,
	RESIZE_FAILED,
	SUCCESS,
	OPERATION_FAIL
}status_t;

/*
Description: Creates a new vector
Args: Size of the elements in the vector
Return Value: Pointer to the new vector
Time Complexity: O(1)
Space Complexity: O(n)
*/
vector_t* VectorCreate(size_t size_of_item);

/*
Description: Free the vector
Args: Pointer to the vector
Return Value: None
Time Complexity: O(1)
Space Complexity: O(1)
*/
void VectorDestroy(vector_t* vector);

/*
Description: Adds an element to the end of the vector, reserving if necessary
Args: Pointer to the vector, item to add
Return Value: Success or fail
Time Complexity: Amortized O(1) (due to reserving, worst case O(n))
Space Complexity: Amortized O(1)
*/
status_t VectorPushBack(vector_t* vector, void* item);

/*
Description: Removes the last element from the vector
Args: Pointer to the vector
Return Value: None
Time Complexity: O(1)
Space Complexity: O(1)
*/
void VectorPopBack(vector_t* vector);

/*
Description: Returns the number of elements in the vector
Args: Pointer to the vector
Return Value: Number of elements in the vector
Time Complexity: O(1)
Space Complexity: O(1)
*/
size_t VectorGetSize(const vector_t* vector);

/*
Description: Returns the capacity of the vector
Args: Pointer to the vector
Return Value: Capacity of the vector
Time Complexity: O(1)
Space Complexity: O(1)
*/
size_t VectorGetCapacity(const vector_t* vector);

/*
Description: Returns a pointer to the element at the specified index
Args: Pointer to the vector, index
Return Value: Pointer to the element
Time Complexity: O(1)
Space Complexity: O(1)
*/
void* VectorGetAccess(vector_t* vector, size_t index);

/*
Description: Reserves capacity for the vector
Args: Pointer to the vector, new capacity
Return Value: Status of the operation
Time Complexity: O(n) (due to memory reallocation)
Space Complexity: O(n)
*/
status_t VectorReserve(vector_t* vector, size_t new_capacity);

/*
Description: Shrinks the vector's capacity to match its size
Args: Pointer to the vector
Return Value: Status of the operation
Time Complexity: O(n) (due to memory reallocation)
Space Complexity: O(n)
*/
status_t VectorShrink(vector_t* vector);

#endif /* end of header guard __VECTOR_H__ */

