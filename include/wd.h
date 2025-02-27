/*
    Version 4.0.0
*/

#ifndef __WD_H__
#define __WD_H__

#include <sys/types.h>
#include <time.h>

typedef enum wd_status
{
    ALLOC_FAIL = 1,
    EXEC_FAILED,
    FORK_FAILED,
    SEM_OPEN_FAILED,
    SCHEDULER_FAILED,
    THREAD_CREATION_FAILED
} wd_status_t;

wd_status_t WDStart(int argc, const char* argv[], size_t interval, unsigned int tolerance);
void WDStop();

#endif /*__WD_H__*/
