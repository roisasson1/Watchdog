/***************************************
*	Author: Roi Sasson		*
*	Date:		1.12.2024	*
*	Approver: 	Sefi Hendry	*
****************************************/

#include <stdlib.h> /* malloc, realloc */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#include "vector.h"

#define GROWTH_FACTOR (2)
#define MIN_SIZE (1)

struct vector
{
	void* items;
	size_t size_of_item;
	size_t capacity;
	size_t size;
};

static status_t ResizeVector(vector_t* vector, size_t new_capacity);

vector_t* VectorCreate(size_t size_of_item)
{
	vector_t* vector = (vector_t*)malloc(sizeof(vector_t));
	
	if(NULL == vector)
	{
		return NULL;
	}
	
	vector->size_of_item = size_of_item;
	vector->items = (void*)malloc(size_of_item);
	if(NULL == vector->items)
	{
		free(vector);
		return NULL;
	}
	
	vector->size = 0;
	vector->capacity = 1;
	
	return vector;
}

void VectorDestroy(vector_t* vector)
{
	free(vector->items);
	free(vector);
}

status_t VectorPushBack(vector_t* vector, void* item)
{
	if (vector->size == vector->capacity)
	{
		VectorReserve(vector, vector->capacity * GROWTH_FACTOR);	
	}
	
	memcpy((char*)(vector->items) + (vector->size * vector->size_of_item), (char*)item, vector->size_of_item);
	++vector->size;
	
	return SUCCESS;
}

void VectorPopBack(vector_t* vector)
{
	if (vector->size <= vector->capacity / GROWTH_FACTOR)
	{
		VectorShrink(vector);
	}

	--vector->size;
}

size_t VectorGetSize(const vector_t* vector)
{
	return vector->size;
}

size_t VectorGetCapacity(const vector_t* vector)
{
	return vector->capacity;
}

void* VectorGetAccess(vector_t* vector, size_t index)
{
	assert(index <= vector->size - 1);

	return((char*)(vector->items) + (index * vector->size_of_item));
}

status_t VectorReserve(vector_t* vector, size_t new_capacity)
{
	if(vector->capacity < new_capacity)
	{
		void* items_ptr = vector->items;
		
		ResizeVector(vector, new_capacity * vector->size_of_item);
		if(NULL == vector->items)
		{
			vector->items = items_ptr;
			return ALLOCATION_FAIL;
		}
		
		vector->capacity = new_capacity;
		return SUCCESS;
	}
	
	return RESIZE_FAILED;
}

status_t VectorShrink(vector_t* vector)
{
	if (MIN_SIZE < vector->capacity)
	{
		ResizeVector(vector, vector->capacity / GROWTH_FACTOR);
		if(vector->size > vector->capacity)
		{
			vector->size = vector->capacity;
		}
		
		return SUCCESS;
	}
	
	return RESIZE_FAILED;
}

static status_t ResizeVector(vector_t* vector, size_t new_capacity)
{
    char* vector_data = (char*)realloc(vector->items, new_capacity * vector->size_of_item);
    
    if (NULL == vector_data)
    {
        return ALLOCATION_FAIL;
    }

    vector->items = (void*)vector_data;
    vector->capacity = new_capacity;

    return SUCCESS;
}