/*
Author: Roi Sasson
Date: 20-12-2024
Reviewer:
*/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "task.h"

#define TIME_FAILURE (-1)
#define SUCCESS (0)
#define FAIL (1)
#define FALSE (0)
#define TRUE (1)

task_t* TaskCreate(operation_t operation, void* args, size_t interval, cleanup_op_t cleanup_op, void* cleanup_args)
{
	task_t* task = (task_t*)malloc(sizeof(task_t));
	
	assert(NULL != operation);
	
	task->id = UIDCreate();
	if (UIDIsEqual(BadUID, task->id))
	{
		free(task);
		return NULL;
	}
	
	task->interval = interval;
	TaskUpdateTimeToRun(task);
	if (TIME_FAILURE == task->time_to_run)
	{
		free(task);
		return NULL;
	}
	
	task->operation = operation;
	task->args = args;
	task->cleanup_op = cleanup_op;
	task->cleanup_args = cleanup_args;
	
	return task;
}

void TaskDestroy(task_t* task)
{
	assert(NULL != task);
	
	if (NULL != task->cleanup_op)
	{
		task->cleanup_op(task->cleanup_args);
	}
	
	free(task);
}

int TaskRun(task_t* task)
{
	assert(NULL != task);
	
	return task->operation(task->args);
}

time_t TaskGetTimeToRun(const task_t* task)
{
	assert(NULL != task);
	
	return task->time_to_run;
}

UID_t TaskGetUID(const task_t* task)
{
	assert(NULL != task);
	
	return task->id;
}

int TaskIsMatch(const task_t* task1, const task_t* task2)
{
	assert(NULL != task1);
	assert(NULL != task2);
	
	return UIDIsEqual(task1->id, task2->id);
}

int TaskUpdateTimeToRun(task_t* task)
{
	time_t timer = time(NULL);
	assert(NULL != task);
	
	if (TIME_FAILURE == timer)
	{
		return FAIL;
	}
	
	task->time_to_run = timer + (time_t)task->interval;
	
	return SUCCESS;
}
