/*
Author: Roi Sasson
Date: 24-12-2024
Reviewer: Or Eliyahu
*/
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <unistd.h> /* sleep */
#include <time.h>   /* time */

#include "task.h" /* task API */
#include "scheduler.h" /* API */

#define FAIL (-1)
#define SUCCESS (0)
#define TRUE (1)
#define FALSE (0)

struct scheduler
{
    pqueue_t* pqueue;
    int is_scheduler_running;
    int is_task_running;
    int is_cleared;
};

static void SchedulerSleepUntilNextTask(scheduler_t* scheduler);
static run_status_t SchedulerHandleTaskExecution(scheduler_t* scheduler);
static int SchedulerComperator(void* task1, void* task2);
static int IsTaskMatchWrapper(void* id, void* task);

scheduler_t* SchedulerCreate(void)
{
    scheduler_t* scheduler = (scheduler_t*)malloc(sizeof(scheduler_t));

    if (NULL == scheduler)
    {
        return NULL;
    }

    scheduler->pqueue = PQCreate(SchedulerComperator);
    if (NULL == scheduler->pqueue)
    {
        free(scheduler);
        return NULL;
    }

    scheduler->is_scheduler_running = TRUE;
    scheduler->is_task_running = FALSE;
    scheduler->is_cleared = FALSE;

    return scheduler;
}

void SchedulerDestroy(scheduler_t* scheduler)
{
    assert(NULL != scheduler);

    SchedulerClear(scheduler);
    PQDestroy(scheduler->pqueue);
    free(scheduler);
}

UID_t SchedulerAddTask(scheduler_t* scheduler, s_operation_t operation, void* args,
                       size_t interval, s_cleanup_op_t cleanup_op, void* cleanup_args)
{
    task_t* task = NULL;

    assert(NULL != scheduler);

    task = TaskCreate(operation, args, interval, cleanup_op, cleanup_args);
    if (NULL == task)
    {
        return BadUID;
    }

    if (FAIL == PQEnqueue(scheduler->pqueue, task))
    {
        TaskDestroy(task);
        return BadUID;
    }

    scheduler->is_cleared = FALSE;

    return TaskGetUID(task);
}

void SchedulerRemove(scheduler_t* scheduler, UID_t task_id)
{
    task_t* task = NULL;

    assert(NULL != scheduler);

    task = (task_t*)PQErase(scheduler->pqueue, &task_id, IsTaskMatchWrapper);
    if (NULL != task)
    {
        TaskDestroy(task);
    }
}

run_status_t SchedulerRun(scheduler_t* scheduler)
{
	run_status_t status = SUCCESSFULL_RUN;
    assert(NULL != scheduler);

    scheduler->is_scheduler_running = TRUE;
    while (!SchedulerIsEmpty(scheduler) && TRUE == scheduler->is_scheduler_running)
    {
        SchedulerSleepUntilNextTask(scheduler);
        status = SchedulerHandleTaskExecution(scheduler);
        if (status != SUCCESSFULL_RUN)
        {
            return status;
        }
    }

    return status;
}

void SchedulerStop(scheduler_t* scheduler)
{
    assert(NULL != scheduler);

    scheduler->is_scheduler_running = FALSE;
}

void SchedulerClear(scheduler_t* scheduler)
{
    assert(NULL != scheduler);

    while (!PQIsEmpty(scheduler->pqueue))
    {
        task_t* task = PQDequeue(scheduler->pqueue);
        if (NULL != task)
        {
            TaskDestroy(task);
        }
    }

    scheduler->is_cleared = TRUE;
}

int SchedulerIsEmpty(scheduler_t* scheduler)
{
    assert(NULL != scheduler);

    return !scheduler->is_task_running && PQIsEmpty(scheduler->pqueue);
}

size_t SchedulerSize(scheduler_t* scheduler)
{
    assert(NULL != scheduler);

    return (size_t)scheduler->is_task_running + PQSize(scheduler->pqueue);
}

static void SchedulerSleepUntilNextTask(scheduler_t* scheduler)
{
    task_t* task_to_run = (task_t*)PQPeek(scheduler->pqueue);
    time_t time_to_run = TaskGetTimeToRun(task_to_run);
    time_t current_time = time(NULL);
    time_t sleep_time = time_to_run - current_time;

    assert(NULL != scheduler);

    if (sleep_time > 0)
    {
        sleep(sleep_time);
    }
}

static run_status_t SchedulerHandleTaskExecution(scheduler_t* scheduler)
{
    int run_result = 0;
    task_t* task_to_run = PQDequeue(scheduler->pqueue);

    assert(NULL != scheduler);

    scheduler->is_task_running = TRUE;
    run_result = TaskRun(task_to_run);

    if (!scheduler->is_cleared && TRUE == run_result)
    {
        if (FAIL == TaskUpdateTimeToRun(task_to_run))
        {
            scheduler->is_task_running = FALSE;
            return TIME_FAILURE;
        }

        if (FAIL == PQEnqueue(scheduler->pqueue, task_to_run))
        {
            scheduler->is_task_running = FALSE;
            return ENQUEUE_FAIL;
        }
    }
    else
    {
        TaskDestroy(task_to_run);
    }

    scheduler->is_task_running = FALSE;

    return SUCCESSFULL_RUN;
}

static int SchedulerComperator(void* task1, void* task2)
{
    time_t time1 = TaskGetTimeToRun((task_t*)task1);
    time_t time2 = TaskGetTimeToRun((task_t*)task2);

    return time1 - time2;
}

static int IsTaskMatchWrapper(void* id, void* task)
{
    return UIDIsEqual(*(UID_t*)id, TaskGetUID((task_t*)task));
}