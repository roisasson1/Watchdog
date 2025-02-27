#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__ 

#include <stddef.h> /* include size_t */
#include "pqueue.h"
#include "uid.h"

typedef enum
{
    STOP = -3,
    ENQUEUE_FAIL,
    TIME_FAILURE,
    SUCCESSFULL_RUN
} run_status_t;

typedef struct scheduler scheduler_t;
typedef int (*s_operation_t)(void* args);
typedef void (*s_cleanup_op_t)(void* cleanup_args);

/*
    Description: Creates a new scheduler
    Args: None
    Return Value: A pointer to the created scheduler
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
scheduler_t* SchedulerCreate(void);

/*
    Description: Destroys the scheduler and releases all associated resources
    Args: A pointer to the scheduler
    Return Value: None
    Time Complexity: O(n)
    Space Complexity: O(1)
*/
void SchedulerDestroy(scheduler_t* scheduler);

/*
    Description: Adds a new task to the scheduler
    Args: 
        scheduler - A pointer to the scheduler
        operation - The task operation to execute
        args - Arguments for the task operation
        interval - The time interval (in seconds) between executions
        cleanup_op - A cleanup function for the task
        cleanup_args - Arguments for the cleanup function
    Return Value: The unique identifier (UID) of the added task
    Time Complexity: O(n)
    Space Complexity: O(1)
*/
UID_t SchedulerAddTask(scheduler_t* scheduler, s_operation_t operation, void* args, size_t interval, s_cleanup_op_t cleanup_op, void* cleanup_args);

/*
    Description: Removes a task from the scheduler based on its UID
    Args: A pointer to the scheduler, The UID of the task to remove
    Return Value: None
    Time Complexity: O(n)
    Space Complexity: O(1)
*/
void SchedulerRemove(scheduler_t* scheduler, UID_t task);

/*
    Description: Clears all tasks from the scheduler
    Args: A pointer to the scheduler
    Return Value: None
    Time Complexity: O(n)
    Space Complexity: O(1)
*/
void SchedulerClear(scheduler_t* scheduler);

/*
    Description: Runs the scheduler and executes tasks in the scheduled order
    Args: A pointer to the scheduler
    Return Value: A status indicating the outcome of the run (SUCCESSFUL_RUN, STOP)
    Time Complexity: O(tasks)
    Space Complexity: O(1)
*/
run_status_t SchedulerRun(scheduler_t* scheduler);

/*
    Description: Stops running the tasks in the scheduler
    Args: A pointer to the scheduler
    Return Value: None
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
void SchedulerStop(scheduler_t* scheduler);

/*
    Description: Checks if the scheduler is empty
    Args: A pointer to the scheduler
    Return Value: 1 if empty, 0 otherwise
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
int SchedulerIsEmpty(scheduler_t* scheduler);

/*
    Description: Returns the number of tasks in the scheduler
    Args: A pointer to the scheduler
    Return Value: The number of tasks in the scheduler
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
size_t SchedulerSize(scheduler_t* scheduler);

#endif /* end of header guard __SCHEDULER_H__ */
