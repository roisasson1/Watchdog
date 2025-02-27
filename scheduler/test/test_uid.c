#include <stdio.h>
#include <string.h>
#include <unistd.h> /* getpid */

#include "uid.h"

static const char *red = "\033[31m";
static const char *green = "\033[32m";
static const char *reset = "\033[0m";

void UIDCreateTest()
{
	const size_t count_tests = 5;
	size_t count_tests_success = count_tests;
	
	UID_t uid1 = UIDCreate();
	UID_t uid2 = UIDCreate();
	time_t timer = time(NULL);

	printf("**UIDCreate test:**\n");
	if (uid1.counter != 0)
	{
		printf("%sTest 1 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (uid1.counter == uid2.counter)
	{
		printf("%sTest 2 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (uid1.time != timer)
	{
		printf("%sTest 3 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (uid1.pid != getpid())
	{
		printf("%sTest 4 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (0 != memcmp(uid1.ip, "192.168.1.106", strlen("192.168.1.110") + 1))
	{
		printf("%sTest 5 failed!\n%s", red, reset);
		--count_tests_success;
	}

	if (count_tests_success == count_tests)
	{
		printf("%s%ld out of %ld tests of UIDCreate: SUCCESS!%s\n", green, count_tests_success, count_tests, reset);
	}
}

void UIDIsEqualTest()
{
	const size_t count_tests = 2;
	size_t count_tests_success = count_tests;
	
	UID_t uid1 = UIDCreate();
	UID_t uid2 = UIDCreate();
	UID_t uid3 = {0};
	
	memcpy(&uid3, &uid1, sizeof(uid1));

	printf("**UIDIsEqual test:**\n");
	if (UIDIsEqual(uid1, uid2) != 0)
	{
		printf("%sTest 1 failed!%s\n", red, reset);
		--count_tests_success;
	}
	
	if (UIDIsEqual(uid1, uid3) != 1)
	{
		printf("%sTest 2 failed!%s\n", red, reset);
		--count_tests_success;
	}

	if (count_tests_success == count_tests)
	{
		printf("%s%ld out of %ld tests of UIDIsEqual: SUCCESS!%s\n", green, count_tests_success, count_tests, reset);
	}
}

int main()
{
	UIDCreateTest();
	UIDIsEqualTest();
	
	return 0;
}
