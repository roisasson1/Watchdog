#include <stdio.h>

#include "scheduler.h"

static const char *red = "\033[31m";
static const char *green = "\033[32m";
static const char *reset = "\033[0m";

static int IsEmptyOp(void* args)
{
	scheduler_t* scheduler = (scheduler_t*)args;
	
	SchedulerIsEmpty(scheduler);
	
	return 0;
}

static int SizeOp(void* args)
{
	scheduler_t* scheduler = (scheduler_t*)args;
	
	SchedulerSize(scheduler);
	
	return 0;
}

static int ClearOp(void* cleanup_args)
{
	scheduler_t* scheduler = (scheduler_t*)cleanup_args;
	
	SchedulerClear(scheduler);
	
	return 1;
}

static int StopOp(void* args)
{
	scheduler_t* scheduler = (scheduler_t*)args;
	SchedulerStop(scheduler);
	
	return 0;
}

static int MultplyBy2(void* x)
{
	*(int*)x *= 2;
	
	return 0; 
}

static int Print(void* x)
{
	printf("%d\n", *(int*)x);
	
	return 0; 
}

void SchedulerCreateTest()
{
	const size_t count_tests = 2;
	size_t count_tests_success = count_tests;
	
	scheduler_t* scheduler = SchedulerCreate();
	UID_t uid = {0};
	size_t interval1 = 5;
	int num = 10;
	
	uid = SchedulerAddTask(scheduler, MultplyBy2, &num, interval1, NULL, NULL);
	SchedulerRemove(scheduler, uid);
	
	printf("**SchedulerCreate test:**\n");
	if (NULL == scheduler)
	{
		printf("%sTest 1 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (0 == SchedulerIsEmpty(scheduler))
	{
		printf("%sTest 2 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (count_tests_success == count_tests)
	{
		printf("%s%ld out of %ld tests of TaskCreate: SUCCESS!%s\n", green, count_tests_success, count_tests, reset);
	}
	
	SchedulerDestroy(scheduler);
}

void SchedulerAddTaskTest()
{
	const size_t count_tests = 2;
	size_t count_tests_success = count_tests;
	
	UID_t uid = {0};
	scheduler_t* scheduler = SchedulerCreate();
	size_t interval1 = 5;
	int num = 10;
	
	printf("**SchedulerAddTask test:**\n");
	uid = SchedulerAddTask(scheduler, MultplyBy2, &num, interval1, NULL, NULL);
	SchedulerAddTask(scheduler, Print, &num, interval1, NULL, NULL);
	
	if (UIDIsEqual(BadUID, uid) != 0)
	{
		printf("%sTest 1 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (2 != SchedulerSize(scheduler))
	{
		printf("%sTest 2 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (count_tests_success == count_tests)
	{
		printf("%s%ld out of %ld tests of SchedulerAddTask: SUCCESS!%s\n", green, count_tests_success, count_tests, reset);
	}
	
	SchedulerDestroy(scheduler);
}

void SchedulerRemoveTest()
{
	const size_t count_tests = 2;
	size_t count_tests_success = count_tests;
	
	UID_t uid1 = {0};
	UID_t uid2 = {0};
	scheduler_t* scheduler = SchedulerCreate();
	size_t interval1 = 5;
	int num = 10;
	
	printf("**SchedulerRemove test:**\n");
	SchedulerAddTask(scheduler, MultplyBy2, &num, interval1, NULL, NULL);
	uid1 = SchedulerAddTask(scheduler, Print, &num, interval1, NULL, NULL);
	SchedulerAddTask(scheduler, MultplyBy2, &num, interval1, NULL, NULL);
	
	SchedulerRemove(scheduler, uid1);
	
	if (2 != SchedulerSize(scheduler))
	{
		printf("%sTest 1 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	SchedulerRemove(scheduler, uid2);
	if (2 != SchedulerSize(scheduler))
	{
		printf("%sTest 2 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (count_tests_success == count_tests)
	{
		printf("%s%ld out of %ld tests of SchedulerRemove: SUCCESS!%s\n", green, count_tests_success, count_tests, reset);
	}
	
	SchedulerDestroy(scheduler);
}

void SchedulerClearTest()
{
	const size_t count_tests = 1;
	size_t count_tests_success = count_tests;
	
	scheduler_t* scheduler = SchedulerCreate();
	int num = 10;
	
	printf("**SchedulerClear test:**\n");
	SchedulerAddTask(scheduler, MultplyBy2, &num, 1, NULL, NULL);
	SchedulerAddTask(scheduler, Print, &num, 2, NULL, NULL);
	SchedulerAddTask(scheduler, ClearOp, scheduler, 3, NULL, NULL);
	SchedulerAddTask(scheduler, MultplyBy2, &num, 4, NULL, NULL);
	SchedulerAddTask(scheduler, MultplyBy2, &num, 5, NULL, NULL);
	
	SchedulerRun(scheduler);
	if (0 != SchedulerSize(scheduler))
	{
		printf("%sTest 1 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (count_tests_success == count_tests)
	{
		printf("%s%ld out of %ld tests of SchedulerClear: SUCCESS!%s\n", green, count_tests_success, count_tests, reset);
	}
	
	SchedulerDestroy(scheduler);
}

void SchedulerRunTest()
{
	const size_t count_tests = 1;
	size_t count_tests_success = count_tests;
	
	scheduler_t* scheduler = SchedulerCreate();
	size_t interval1 = 1;
	int num = 10;
	
	printf("**SchedulerRun test:**\n");
	/* tests with the same interval */
	SchedulerAddTask(scheduler, MultplyBy2, &num, interval1, NULL, NULL);
	SchedulerAddTask(scheduler, Print, &num, interval1, NULL, NULL);
	SchedulerAddTask(scheduler, MultplyBy2, &num, interval1, NULL, NULL);
	SchedulerAddTask(scheduler, Print, &num, interval1, NULL, NULL);
	SchedulerAddTask(scheduler, MultplyBy2, &num, interval1, NULL, NULL);
	SchedulerAddTask(scheduler, Print, &num, interval1, NULL, NULL);
	
	SchedulerRun(scheduler);
	
	if (0 != SchedulerSize(scheduler))
	{
		printf("%sTest 1 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (count_tests_success == count_tests)
	{
		printf("%s%ld out of %ld tests of SchedulerRun: SUCCESS!%s\n", green, count_tests_success, count_tests, reset);
	}
	
	SchedulerDestroy(scheduler);
}

void SchedulerStopTest()
{
	const size_t count_tests = 2;
	size_t count_tests_success = count_tests;
	
	scheduler_t* scheduler = SchedulerCreate();
	int num = 10;
	
	printf("**SchedulerStop test:**\n");
	/* tests with different interval */
	SchedulerAddTask(scheduler, MultplyBy2, &num, 1, NULL, NULL);
	SchedulerAddTask(scheduler, Print, &num, 2, NULL, NULL);
	SchedulerAddTask(scheduler, StopOp, scheduler, 3, NULL, NULL);
	SchedulerAddTask(scheduler, Print, &num, 4, NULL, NULL);
	SchedulerAddTask(scheduler, MultplyBy2, &num, 5, NULL, NULL);
	SchedulerAddTask(scheduler, Print, &num, 6, NULL, NULL);
	
	SchedulerRun(scheduler);
	printf("%ld\n", SchedulerSize(scheduler));
	if (2 != SchedulerSize(scheduler))
	{
		printf("%sTest 1 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	SchedulerRun(scheduler);
	if (0 != SchedulerSize(scheduler))
	{
		printf("%sTest 2 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (count_tests_success == count_tests)
	{
		printf("%s%ld out of %ld tests of SchedulerStop: SUCCESS!%s\n", green, count_tests_success, count_tests, reset);
	}
	
	SchedulerDestroy(scheduler);
}

void SchedulerSizeTest()
{
	const size_t count_tests = 3;
	size_t count_tests_success = count_tests;
	
	scheduler_t* scheduler = SchedulerCreate();
	int num = 10;
	
	printf("**SchedulerSize test:**\n");
	/* tests with different interval */
	SchedulerAddTask(scheduler, SizeOp, scheduler, 0, NULL, NULL);
	SchedulerAddTask(scheduler, MultplyBy2, &num, 1, NULL, NULL);
	SchedulerAddTask(scheduler, Print, &num, 2, NULL, NULL);
	SchedulerAddTask(scheduler, SizeOp, scheduler, 3, NULL, NULL);
	SchedulerAddTask(scheduler, IsEmptyOp, scheduler, 4, NULL, NULL);
	SchedulerAddTask(scheduler, Print, &num, 5, NULL, NULL);
	SchedulerAddTask(scheduler, MultplyBy2, &num, 6, NULL, NULL);
	SchedulerAddTask(scheduler, Print, &num, 7, NULL, NULL);
	SchedulerAddTask(scheduler, IsEmptyOp, scheduler, 8, NULL, NULL);
	
	if (9 != SchedulerSize(scheduler))
	{
		printf("%sTest 1 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	SchedulerRun(scheduler);
	
	if (0 != SchedulerSize(scheduler))
	{
		printf("%sTest 2 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (1 != SchedulerIsEmpty(scheduler))
	{
		printf("%sTest 3 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (count_tests_success == count_tests)
	{
		printf("%s%ld out of %ld tests of SchedulerSize: SUCCESS!%s\n", green, count_tests_success, count_tests, reset);
	}
	
	SchedulerDestroy(scheduler);
}

int main()
{
	SchedulerCreateTest();
	SchedulerAddTaskTest();
	SchedulerRemoveTest();
	SchedulerClearTest();
	SchedulerRunTest();
	SchedulerStopTest();
	SchedulerSizeTest();
	
	return 0;
}
