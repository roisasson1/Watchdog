/*
Author: Roi Sasson
Date: 11-12-2024
Reviewer: Daniel Karamzin
*/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "sdll.h"

struct list
{
	dll_t* list;
	comperator_t comperator;
};

comperator_t g_comperator = NULL;

static int Wrapper(void* num1, void* num2);
static sdll_iterator_t DToSIterator(dll_iterator_t d_iterator, dll_t* d_list);
static dll_iterator_t SToDIterator(sdll_iterator_t s_iterator);

sdll_t* SdllCreate(comperator_t comperator)
{
	sdll_t* sorted_list = (sdll_t*)malloc(sizeof(sdll_t));
	
	if (NULL == sorted_list)
	{
		return NULL;
	}
	
	sorted_list->list = DllCreate();
	if (NULL == sorted_list->list)
	{
		free(sorted_list);
		return NULL;
	}
	
	sorted_list->comperator = comperator;
	
	return sorted_list;
}

void SdllDestroy(sdll_t* s_list)
{
	assert(NULL != s_list);
	
	DllDestroy(s_list->list);
	free(s_list);
}

void* SdllPopBack(sdll_t* s_list)
{
	assert(NULL != s_list);
	
	return DllPopBack(s_list->list);
}

void* SdllPopFront(sdll_t* s_list)
{
	assert(NULL != s_list);
	
	return DllPopFront(s_list->list);
}

sdll_iterator_t SdllInsert(sdll_t* s_list, void* data)
{
	dll_iterator_t curr = NULL;
	
	assert(NULL != s_list);
	
	curr = DllBegin(s_list->list);
	while (!DllIsEqual(curr, DllEnd(s_list->list)) && 0 >= s_list->comperator(DllGetData(curr), data))
	{
		curr = DllGetNext(curr);
	}
	
	curr = DllInsert(s_list->list, curr, data);
	
	return DToSIterator(curr, s_list);
}

sdll_iterator_t SdllRemove(sdll_iterator_t s_iterator)
{
	assert(NULL != s_iterator.iterator);
	
	return DToSIterator(DllRemove(SToDIterator(s_iterator)), s_iterator.list);
}

sdll_iterator_t SdllFind(sdll_iterator_t from, sdll_iterator_t to, void* data, sdll_t* s_list)
{
	assert(NULL != s_list);
	assert(NULL != from.list);
	assert(NULL != from.iterator);
	assert(NULL != to.list);
	assert(NULL != to.iterator);
	assert(from.list == to.list);
	
	g_comperator = s_list->comperator;
	
	return DToSIterator(DllFind(SToDIterator(from), SToDIterator(to), data, Wrapper), (sdll_t*)s_list);
}

sdll_iterator_t SdllFindIf(sdll_iterator_t from, sdll_iterator_t to, void* data, int (*IsMatch)(void*, void*))
{
	assert(NULL != from.list);
	assert(NULL != from.iterator);
	assert(NULL != to.list);
	assert(NULL != to.iterator);
	
	return DToSIterator(DllFind(SToDIterator(from), SToDIterator(to), data, IsMatch), from.list);
}

size_t SdllSize(const sdll_t* s_list)
{
	assert(NULL != s_list);
	
	return DllSize(s_list->list);
}

int SdllIsEmpty(const sdll_t* s_list)
{
	assert(NULL != s_list);
	
	return DllIsEmpty(s_list->list);
}

void* SdllGetData(const sdll_iterator_t s_iterator)
{
	assert(NULL != s_iterator.list);
	assert(NULL != s_iterator.iterator);
	
	return DllGetData(SToDIterator(s_iterator));
}

sdll_iterator_t SdllGetNext(const sdll_iterator_t s_iterator)
{
	assert(NULL != s_iterator.list);
	assert(NULL != s_iterator.iterator);
	
	return DToSIterator(DllGetNext(SToDIterator(s_iterator)), s_iterator.list);
}

sdll_iterator_t SdllGetPrev(const sdll_iterator_t s_iterator)
{
	assert(NULL != s_iterator.list);
	assert(NULL != s_iterator.iterator);
	
	return DToSIterator(DllGetPrev(SToDIterator(s_iterator)), s_iterator.list);
}

sdll_iterator_t SdllBegin(const sdll_t* s_list)
{
	assert(NULL != s_list);
	
	return DToSIterator(DllBegin(s_list->list), (sdll_t*)s_list);
}

sdll_iterator_t SdllEnd(const sdll_t* s_list)
{
	assert(NULL != s_list);
	
	return DToSIterator(DllEnd(s_list->list), (sdll_t*)s_list);
}

status_t SdllForEach(sdll_iterator_t from, sdll_iterator_t to, int (*OperationFunc)(void*, void*), void* args)
{
	assert(NULL != from.list);
	assert(NULL != from.iterator);
	assert(NULL != to.list);
	assert(NULL != to.iterator);
	
	return DllForEach(SToDIterator(from), SToDIterator(to), OperationFunc, args);
}

int SdllIsEqual(sdll_iterator_t s_iterator1, sdll_iterator_t s_iterator2)
{
	assert(NULL != s_iterator1.list);
	assert(NULL != s_iterator1.iterator);
	assert(NULL != s_iterator2.list);
	assert(NULL != s_iterator2.iterator);
	
	return DllIsEqual(SToDIterator(s_iterator1), SToDIterator(s_iterator2));
}

void SdllMerge(sdll_t* s_list1, sdll_t* s_list2)
{
	sdll_iterator_t curr1 = {0};
	sdll_iterator_t curr2 = {0};
	sdll_iterator_t from = {0};
	sdll_iterator_t to = {0};
	
	assert(NULL != s_list1);
	assert(NULL != s_list2);
	
	curr1 = SdllBegin(s_list1);
	while (!SdllIsEqual(curr1, SdllEnd(s_list1)))
	{
		curr2 = SdllBegin(s_list2);
		from = curr2;
		to = curr2;
		
		/* while the list2 elements are smaller than the element in list1 */
		while (s_list2->comperator(SdllGetData(curr1), SdllGetData(to)) > 0 && !SdllIsEqual(curr1, SdllEnd(s_list1)))
		{
			to = SdllGetNext(to);
		}

		if (!SdllIsEqual(from, to))
		{
			DllSplice(SToDIterator(curr1), SToDIterator(from), SToDIterator(to));
		}
		
		curr1 = SdllGetNext(curr1);
	}
	
	/* Splice the rest of list2 to end of list1 */
	DllSplice(SToDIterator(curr1), SToDIterator(SdllBegin(s_list2)), SToDIterator(SdllEnd(s_list2)));
}

static int Wrapper(void* num1, void* num2)
{
	assert(NULL != g_comperator);
	
	return g_comperator(num1, num2) == 0 ? 1 : 0; 
}

static dll_iterator_t SToDIterator(sdll_iterator_t s_iterator)
{
	return s_iterator.iterator;
}

static sdll_iterator_t DToSIterator(dll_iterator_t d_iterator, dll_t* d_list)
{
	sdll_iterator_t s_iterator;
	
	assert(NULL != d_list);
	
	s_iterator.iterator = d_iterator;
	#ifndef NDEBUG
	s_iterator.list = d_list;
	#else
	(void)d_list;
	#endif
	
	return s_iterator;
}
