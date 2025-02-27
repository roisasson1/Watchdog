/*
Author: Roi Sasson
Date: 08-12-2024
Reviewer: Sefi Hendry
*/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "dll.h"


struct node
{
	void* data;
	dll_node_t* next;
	dll_node_t* prev;
};

struct list
{
	dll_node_t head;
	dll_node_t tail;
};

static int GetSize(void* data, void* args);

dll_t* DllCreate()
{
	dll_t* list = (dll_t*)malloc(sizeof(dll_t));
	
	if (NULL == list)
	{
		return NULL;
	}
	
	list->head.data = NULL;
	list->head.next = &list->tail;
	list->head.prev = NULL;
	
	list->tail.data = NULL;
	list->tail.next = NULL;
	list->tail.prev = &list->head;
	
	return list;
}

void DllDestroy(dll_t* list)
{
	dll_iterator_t runner = NULL;
	dll_iterator_t next_runner = NULL;
	
	assert(NULL != list);
	
	runner = DllGetPrev(DllBegin(list));
	while (!DllIsEqual(DllGetNext(runner), DllEnd(list)))
	{
		next_runner = DllGetNext(runner);
		DllRemove(next_runner);
	}
	
	runner->next->prev = NULL;
	runner->prev = NULL;
	free(list);
}


dll_iterator_t DllPushFront(dll_t* list, void* data)
{
	assert(NULL != list);
	
	return DllInsert(list, DllBegin(list), data);
}

dll_iterator_t DllPushBack(dll_t* list, void* data)
{
	assert(NULL != list);
	
	return DllInsert(list, DllEnd(list), data);
}

void* DllPopBack(dll_t* list)
{
	void* data = NULL;
	
	assert(NULL != list);
	
	data = DllGetData(DllGetPrev(DllEnd(list)));
	DllRemove(DllGetPrev(DllEnd(list)));
	
	return data;
}

void* DllPopFront(dll_t* list)
{
	void* data = NULL;
	
	assert(NULL != list);
	
	data = DllGetData(DllBegin(list));
    	DllRemove(DllBegin(list));
	
	return data;
}

dll_iterator_t DllInsert(dll_t* list, dll_iterator_t iterator, void* data)
{
	dll_iterator_t new_node = NULL;
	
	assert(NULL != list);
	assert(NULL != iterator);
	
	new_node = (dll_iterator_t)malloc(sizeof(dll_node_t));
	
	if (NULL == new_node)
	{
		return DllEnd(list);
	}
	
	new_node->next = iterator;
	new_node->prev = DllGetPrev(iterator);
	new_node->data = data;
	iterator->prev->next = new_node;
	iterator->prev = new_node;
	
	return new_node;
}

dll_iterator_t DllRemove(dll_iterator_t iterator)
{
	dll_iterator_t next_node = NULL;
	assert(NULL != iterator);
	
	next_node = DllGetNext(iterator);
	
	iterator->prev->next = DllGetNext(iterator);
	iterator->next->prev = DllGetPrev(iterator);
	iterator->next = NULL;
	iterator->prev = NULL;
	free(iterator);
	
	return next_node;
}

dll_iterator_t DllFind(dll_iterator_t from, dll_iterator_t to, void* data, int (IsMatch)(void*, void*))
{
	dll_iterator_t curr = NULL;
	
	assert(NULL != to);
	assert(NULL != from);
	
	curr = from;
	while (!DllIsEqual(curr, to))
	{
		if (IsMatch(DllGetData(curr), data))
		{
			return curr;
		}
		
		curr = DllGetNext(curr);
	}
	
	return to;
}

status_t DllMultiFind(dll_t* out_params, dll_iterator_t from, dll_iterator_t to, void* data, int (Is_Match)(void*, void*))
{
	status_t return_status = SUCCESS;
	dll_iterator_t found_node = NULL;
	
	assert(NULL != out_params);
	assert(NULL != from);
	assert(NULL != to);
	
	while (!DllIsEqual(from, to))
	{
		found_node = DllFind(from, to, data, Is_Match);
		if (DllEnd(out_params) == DllPushBack(out_params, DllGetData(found_node)))
		{
			return_status = OPERATION_FAIL;
			break;
		}
		
		from = DllGetNext(found_node);
	}
	
	return return_status;
}

size_t DllSize(const dll_t* list)
{
	size_t count = 0;
	
	assert(NULL != list);
	
	DllForEach(DllBegin(list), DllEnd(list), GetSize, &count);
	
	return count;
}

int DllIsEmpty(const dll_t* list)
{
	assert(NULL != list);
	
	return DllIsEqual(DllBegin(list), DllEnd(list));
}

void* DllGetData(const dll_iterator_t iterator)
{
	assert(NULL != iterator);
	
	return iterator->data;
}

void DllSetData(dll_iterator_t iterator, void* data)
{
	assert(NULL != iterator);
	
	iterator->data = data;
}

dll_iterator_t DllGetNext(const dll_iterator_t iterator)
{
	assert(NULL != iterator);
	
	return iterator->next;
}

dll_iterator_t DllGetPrev(const dll_iterator_t iterator)
{
	assert(NULL != iterator);
	
	return iterator->prev;
}

dll_iterator_t DllBegin(const dll_t* list)
{
	assert(NULL != list);
	
	return (dll_iterator_t)(&list->head)->next;
}

dll_iterator_t DllEnd(const dll_t* list)
{
	assert(NULL != list);
	
	return (dll_iterator_t)&list->tail;
}

status_t DllForEach(dll_iterator_t from, dll_iterator_t to, int (OperationFunc)(void*, void*), void* args)
{
	dll_iterator_t curr = NULL;
	
	assert(NULL != to);
	assert(NULL != from);
	
	curr = from;
	while (!DllIsEqual(curr, to))
	{
		if (SUCCESS != OperationFunc(DllGetData(curr), args))
		{
			return OPERATION_FAIL;
		}
		
		curr = DllGetNext(curr);
	}
	
	return SUCCESS;
}

int DllIsEqual(dll_iterator_t iterator1, dll_iterator_t iterator2)
{
	return iterator1 == iterator2;
}

void DllSplice(dll_iterator_t iterator, dll_iterator_t from, dll_iterator_t to)
{
	dll_iterator_t prev_node_list1 = NULL;
	dll_iterator_t next_node_list2 = NULL;
	
	assert(NULL != iterator);
	assert(NULL != from);
	assert(NULL != to);
	
	to = DllGetPrev(to);
	
	/* save edges of list1 and list2 */
	prev_node_list1 = DllGetPrev(iterator);
	next_node_list2 = DllGetNext(to);
	
	/* update begin and end of list2 */
	from->prev->next = next_node_list2;
	next_node_list2->prev = DllGetPrev(from);
	
	/* update 'to' and 'iterator' pointers */
	iterator->prev = to;
	to->next = iterator;
	
	/* update 'prev list1' and 'from' pointers */
	prev_node_list1->next = from;
	from->prev = prev_node_list1;
}

static int GetSize(void* data, void* args)
{
	(void)data;
	
	++(*(size_t*)args);
	
	return SUCCESS;
} 
