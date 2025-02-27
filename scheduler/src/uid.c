/*
Author: Roi Sasson
Date: 19-12-2024
Reviewer: Chen Sasson
*/
#include <string.h> /* memcmp */
#include <ifaddrs.h> /* ifaddr */
#include <arpa/inet.h> /* inet_ntop */
#include <unistd.h> /* getpid */
#include <pthread.h> /* pthread_mutex */

#include "uid.h" /* API */

#define LOCAL_NAME "lo"

typedef enum
{
	SUCCESS,
	FAIL
} status_t;

static size_t counter_uid = 0;
static pthread_mutex_t uid_mutex = PTHREAD_MUTEX_INITIALIZER;
const UID_t BadUID = {0};

static status_t GetIPAddress(unsigned char* ipBuffer, size_t bufferSize)
{
	struct ifaddrs* ifaddrs = NULL;
	struct ifaddrs* runner = NULL;

	if (-1 == getifaddrs(&ifaddrs))
	{
		return FAIL;
	}
	
	runner = ifaddrs;
	while (NULL != runner)
	{
		if (runner->ifa_addr->sa_family == AF_INET 
			&& 0 != strcmp((const char*)runner->ifa_name, LOCAL_NAME))
		{
			struct sockaddr_in* pAddr = (struct sockaddr_in*)runner->ifa_addr;
			if (NULL != inet_ntop(AF_INET, &pAddr->sin_addr, (char*)ipBuffer, bufferSize))
			{
				break;
			}
		}
		
		runner = runner->ifa_next;
	}

	freeifaddrs(ifaddrs);
	runner = NULL;
	
	return SUCCESS;
}

UID_t UIDCreate()
{
	UID_t uid = {0};

	if (SUCCESS != GetIPAddress(uid.ip, sizeof(uid.ip)))
	{
		return BadUID;
	}
	
	pthread_mutex_lock(&uid_mutex);
	uid.counter = counter_uid++;
	uid.pid = getpid();
	uid.time = time(NULL);
	pthread_mutex_unlock(&uid_mutex);

	return uid;
}

int UIDIsEqual(UID_t uid1, UID_t uid2)
{
	return uid1.counter == uid2.counter &&
		uid1.pid == uid2.pid &&
		(0 == difftime(uid1.time, uid2.time)) &&
		(0 == memcmp(&uid1, &uid2, sizeof(uid1)));
}
