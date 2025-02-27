#include <stdio.h>
#include <string.h>
#include <unistd.h> /* getpid */
#include <stdlib.h>

#include "task.h"

static const char *red = "\033[31m";
static const char *green = "\033[32m";
static const char *reset = "\033[0m";

static int IsGreater(void* a)
{
	return *(int*)a;
}

static int MultplyBy2(void* x)
{
	*(int*)x *= 2;
	return 0; 
}


void TaskCreateTest()
{
	const size_t count_tests = 5;
	size_t count_tests_success = count_tests;
	
	size_t interval = 2;
	
	task_t* task = TaskCreate(IsGreater, NULL, interval, NULL, NULL);
	time_t timer = time(NULL);
	UID_t uid = {0};
	
	printf("**TaskCreate test:**\n");
	memcpy(&uid, &task->id, sizeof(uid));
	if (UIDIsEqual(TaskGetUID(task), uid) != 1)
	{
		printf("%sTest 1 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (TaskGetTimeToRun(task) != timer + (time_t)interval)
	{
		printf("%sTest 2 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (task->interval != 2)
	{
		printf("%sTest 3 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (task->operation != IsGreater)
	{
		printf("%sTest 4 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (task->args != NULL)
	{
		printf("%sTest 5 failed!%s\n", red, reset);
		--count_tests_success;
	}

	if (count_tests_success == count_tests)
	{
		printf("%s%ld out of %ld tests of TaskCreate: SUCCESS!%s\n", green, count_tests_success, count_tests, reset);
	}
	
	TaskDestroy(task);
}

void TaskIsMatchTest()
{
	const size_t count_tests = 2;
	size_t count_tests_success = count_tests;
	
	size_t interval1 = 2;
	size_t interval2 = 4;
	
	task_t* task1 = TaskCreate(IsGreater, NULL, interval1, NULL, NULL);
	task_t* task2 = TaskCreate(IsGreater, NULL, interval2, NULL, NULL);
	task_t* task3 = NULL;
	
	memcpy(&task3, &task1, sizeof(task1));
	
	printf("**TaskIsMatch test:**\n");
	if (TaskIsMatch(task1, task2) != 0)
	{
		printf("%sTest 1 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (TaskIsMatch(task1, task3) != 1)
	{
		printf("%sTest 2 failed!%s\n", red, reset);
		--count_tests_success;
	}

	if (count_tests_success == count_tests)
	{
		printf("%s%ld out of %ld tests of TaskIsMatch: SUCCESS!%s\n", green, count_tests_success, count_tests, reset);
	}
	
	TaskDestroy(task1);
	TaskDestroy(task2);
	task3 = NULL;
}

void TaskRunTest()
{
	const size_t count_tests = 2;
	size_t count_tests_success = count_tests;
	
	task_t* task1 = NULL;
	size_t interval1 = 2;
	int num = 10;
	
	task1 = TaskCreate(MultplyBy2, &num, interval1, NULL, NULL);
	printf("**TaskRun test:**\n");
	if (0 != TaskRun(task1))
	{
		printf("%sTest 1 failed!%s\n", red, reset);
		--count_tests_success;
	}

	if (count_tests_success == count_tests)
	{
		printf("%s%ld out of %ld tests of TaskRun: SUCCESS!%s\n", green, count_tests_success, count_tests, reset);
	}
	
	TaskDestroy(task1);
}

void TaskTimeToRunTest()
{
	const size_t count_get_tests = 1;
	const size_t count_update_tests = 1;
	size_t count_get_tests_success = count_get_tests;
	size_t count_update_tests_success = count_update_tests;
	
	task_t* task1 = NULL;
	size_t interval1 = 2;
	int num = 10;
	time_t curr_time = 0;
	
	task1 = TaskCreate(MultplyBy2, &num, interval1, NULL, NULL);
	printf("**TaskGetTimeToRun test:**\n");
	
	curr_time = TaskGetTimeToRun(task1);
	if (curr_time != time(NULL) + (time_t)interval1)
	{
		printf("%sTest 1 failed!%s\n", red, reset);
		--count_get_tests_success;
	}
	
	if (count_get_tests_success == count_get_tests)
	{
		printf("%s%ld out of %ld tests of TaskGetTimeToRun: SUCCESS!%s\n", green, count_get_tests_success, count_get_tests, reset);
	}
	
	printf("**TaskUpdateTimeToRun test:**\n");
	sleep(1);
	TaskUpdateTimeToRun(task1);
	
	if(curr_time + 1 != TaskGetTimeToRun(task1))
	{
		printf("%sTest 2 failed!%s\n", red, reset);
		--count_update_tests_success;
	}

	if (count_update_tests_success == count_update_tests)
	{
		printf("%s%ld out of %ld tests of TaskUpdateTimeToRun: SUCCESS!%s\n", green, count_update_tests_success, count_update_tests, reset);
	}
	
	TaskDestroy(task1);
}

static int AllocArr(void* arg)
{
	*((int**)arg) = (int*)malloc(sizeof(int) * 5);

	return 0;
}

static void FreeWrapper(void* arg)
{
	free(*((int**)arg));
}

void CleanupTaskTest()
{
	int* arr = NULL;
	task_t* task = NULL;
	
	printf("**CleanupTask test:**\n");
	task = TaskCreate(AllocArr, &arr, 1, FreeWrapper, &arr);
	TaskRun(task);
	TaskDestroy(task);
	
	printf("%sCleanupTask: SUCCESS!%s\n", green, reset);
}

int main()
{
	TaskCreateTest();
	TaskIsMatchTest();
	TaskRunTest();
	TaskTimeToRunTest();
	CleanupTaskTest();
	
	return 0;
}
