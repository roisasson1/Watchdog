#ifndef __TASK_H__
#define __TASK_H__ 

#include "uid.h"

typedef int (*operation_t)(void* args);
typedef void (*cleanup_op_t)(void* cleanup_args);

typedef struct task
{
    UID_t id;
    time_t time_to_run;
    size_t interval;
    operation_t operation;
    void* args;
    cleanup_op_t cleanup_op;
    void* cleanup_args;
} task_t;

/*
    Description: Creates a new task with the given parameters
    Args: 
        operation - A function to perform the task's operation
        args - Arguments to pass to the operation function
        interval - Time interval (in seconds) between executions
        cleanup_op - A function to perform cleanup operations
        cleanup_args - Arguments to pass to the cleanup function
    Return Value: A pointer to the created task
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
task_t* TaskCreate(operation_t operation, void* args, size_t interval, cleanup_op_t cleanup_op, void* cleanup_args);

/*
    Description: Destroys a task and releases all associated resources
    Args: A pointer to the task
    Return Value: None
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
void TaskDestroy(task_t* task);

/*
    Description: Runs the operation associated with the task
    Args: A pointer to the task
    Return Value: The result of the operation function
    Time Complexity: Depends on the implementation of the operation function
    Space Complexity: Depends on the implementation of the operation function
*/
int TaskRun(task_t* task);

/*
    Description: Retrieves the next execution time of the task
    Args: A pointer to the task
    Return Value: The time to run
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
time_t TaskGetTimeToRun(const task_t* task);

/*
    Description: Retrieves the unique identifier (UID) of the task
    Args: A pointer to the task
    Return Value: The UID of the task
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
UID_t TaskGetUID(const task_t* task);

/*
    Description: Compares two tasks to check if they match
    Args: A pointer to the first task, A pointer to the second task
    Return Value: 1 if the tasks match, 0 otherwise
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
int TaskIsMatch(const task_t* task1, const task_t* task2);

/*
    Description: Updates the next execution time of the task based on its interval
    Args: A pointer to the task
    Return Value: 1 if the time was updated successfully, 0 otherwise
    Time Complexity: O(1)
    Space Complexity: O(1)
*/
int TaskUpdateTimeToRun(task_t* task);

#endif /* end of header guard */
