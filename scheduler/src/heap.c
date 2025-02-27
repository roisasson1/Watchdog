/*
Author: Roi Sasson
Date: 18-01-2025
Reviewer: Matan Tabachnik
*/
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "heap.h" /* API */

#define WORD_SIZE (sizeof(void*))

struct heap
{
    vector_t* vector;
    heap_compare_func_t compare_func;
};

static void HeapifyUp(heap_t* heap);
static void HeapifyDown(heap_t* heap);
static void* GetRightChild(heap_t* heap, size_t index);
static size_t GetMinChildIndex(heap_t* heap, void* left_child, void* right_child, size_t i);
static void* FindMatch(heap_t* heap, heap_is_match_t is_match, void* params, size_t* match_index);
static void SwapAndRemoveElement(heap_t* heap, size_t index);
static void SwapElementData(void* a, void* b, size_t size);

heap_t* HeapCreate(heap_compare_func_t compare_func)
{
    heap_t* heap = (heap_t*)malloc(sizeof(heap_t));
    vector_t* vector = NULL;

    if (NULL == heap)
    {
        return NULL;
    }

    vector = VectorCreate(WORD_SIZE);
    if (NULL == vector)
    {
        free(heap);
        return NULL;
    }

    heap->vector = vector;
    heap->compare_func = compare_func;
    return heap;
}

void HeapDestroy(heap_t* heap)
{
    assert(NULL != heap);
    assert(NULL != heap->vector);

    VectorDestroy(heap->vector);
    free(heap);
}

int HeapPush(heap_t* heap, void* data)
{
    int result = 0;

    assert(NULL != heap);
    assert(NULL != data);

    result = VectorPushBack(heap->vector, &data);
    HeapifyUp(heap);

    return result;
}

void* HeapPeek(const heap_t* heap)
{
    assert(NULL != heap);
    assert(NULL != heap->vector);

    return !HeapIsEmpty(heap) ? *(void**)VectorGetAccess(heap->vector, 0) : NULL;
}

void HeapPop(heap_t* heap)
{
    void* last_element = NULL;
    void* first_element = NULL;

    assert(NULL != heap);
    assert(NULL != heap->vector);

    if (HeapIsEmpty(heap))
    {
        return;
    }

    first_element = VectorGetAccess(heap->vector, 0);
    last_element = VectorGetAccess(heap->vector, VectorGetSize(heap->vector) - 1);

    SwapElementData(first_element, last_element, WORD_SIZE);
    VectorPopBack(heap->vector);
    HeapifyDown(heap);
}

void* HeapRemove(heap_t* heap, heap_is_match_t is_match, void* params)
{
    void* removed_data = NULL;
    size_t match_index = 0;

    assert(NULL != heap);
    assert(NULL != heap->vector);
    assert(NULL != is_match);

    removed_data = FindMatch(heap, is_match, params, &match_index);
    if (removed_data != NULL)
    {
        removed_data = *(void**)removed_data;
        SwapAndRemoveElement(heap, match_index);
    }

    return removed_data;
}

int HeapIsEmpty(const heap_t* heap)
{
    assert(NULL != heap);
    assert(NULL != heap->vector);

    return 0 == VectorGetSize(heap->vector);
}

size_t HeapSize(const heap_t* heap)
{
    assert(NULL != heap);
    assert(NULL != heap->vector);

    return VectorGetSize(heap->vector);
}

static void HeapifyUp(heap_t* heap)
{
    void* current = NULL;
    void* parent = NULL;
    size_t index = HeapSize(heap) - 1;
    size_t parent_index = 0;

    assert(NULL != heap);
    assert(NULL != heap->vector);

    while (index > 0)
    {
        parent_index = (index - 1) / 2;
        current = VectorGetAccess(heap->vector, index);
        parent = VectorGetAccess(heap->vector, parent_index);

        /* stop if curr element >= parent */
        if (heap->compare_func(*(void**)current, *(void**)parent) >= 0)
        {
            break;
        }

        SwapElementData(current, parent, WORD_SIZE);
        index = parent_index;
    }
}

static void HeapifyDown(heap_t* heap)
{
    void* current = NULL;
    void* left_child = NULL;
    void* right_child = NULL;
    void* min_child = NULL;
    size_t min_child_index = 0;
    size_t i = 0;

    assert(NULL != heap);
    assert(NULL != heap->vector);

    while ((2 * i) + 1 < HeapSize(heap))
    {
        current = VectorGetAccess(heap->vector, i);
        left_child = VectorGetAccess(heap->vector, (2 * i) + 1);
        right_child = GetRightChild(heap, i);

        min_child_index = GetMinChildIndex(heap, left_child, right_child, i);
        min_child = VectorGetAccess(heap->vector, min_child_index);

        /* stop if curr element <= MIN(left, right) */
        if (heap->compare_func(*(void**)current, *(void**)min_child) <= 0)
        {
            break;
        }

        SwapElementData(current, min_child, WORD_SIZE);
        i = min_child_index;
    }
}

static void* GetRightChild(heap_t* heap, size_t index)
{
    assert(NULL != heap);
    assert(NULL != heap->vector);
    return ((2 * index) + 2 < HeapSize(heap))
             ? VectorGetAccess(heap->vector, (2 * index) + 2) : NULL;
}

static size_t GetMinChildIndex(heap_t* heap, void* left_child, void* right_child, size_t i)
{
    assert(NULL != heap);
    return (right_child && heap->compare_func(*(void**)left_child, *(void**)right_child) > 0)
             ? (2 * i) + 2 : (2 * i) + 1;
}

static void* FindMatch(heap_t* heap, heap_is_match_t is_match, void* params, size_t* match_index)
{
    size_t size = HeapSize(heap);
    size_t i = 0;

    assert(NULL != heap);
    assert(NULL != heap->vector);
    assert(NULL != is_match);

    for (i = 0; i < size; ++i)
    {
        void* current = VectorGetAccess(heap->vector, i);
        if (is_match(*(void**)current, params))
        {
            *match_index = i;
            return current;
        }
    }

    return NULL;
}

static void SwapAndRemoveElement(heap_t* heap, size_t index)
{
    size_t size = HeapSize(heap);
    void* last_element = VectorGetAccess(heap->vector, size - 1);

    assert(NULL != heap);
    assert(NULL != heap->vector);

    if (index < size - 1)
    {
        void* current = VectorGetAccess(heap->vector, index);
        SwapElementData(current, last_element, WORD_SIZE);
        VectorPopBack(heap->vector);
        HeapifyDown(heap);
    }
    else
    {
        VectorPopBack(heap->vector);
    }
}

static void SwapElementData(void* a, void* b, size_t size)
{
    size_t i = 0;
    
    for (i = 0; i < size ;++i)
    {
        char temp = *((char*)a + i);
        *((char*)a + i) = *((char*)b + i);
        *((char*)b + i) = temp;
    }
}