#ifndef WD_COMMON_H
#define WD_COMMON_H

#include <semaphore.h> /* sem_t */
#include <stdatomic.h> /* atomic_int */

#include "scheduler.h" /* scheduler API */

#define TRUE (1)
#define FALSE (0)
#define CONTINUE (1)
#define SUCCESS (0)
#define FAIL (-1)
#define MAX_PID_DIGITS (15)
#define BUFFER_LEN (50)

/* Semaphore Names */
#define WD_SEM "/wd_semaphore"
#define USER_SEM "/user_semaphore"

/* Process Names */
#define WD_PROCESS "./wd_process.out"
#define USER_PROCESS "./user_wd.out"

/* Environment variable */
#define PID_ENV "PID_ENV"

typedef struct watchdog_data
{
    scheduler_t* scheduler;
    char** args;
    size_t interval;
    unsigned int tolerance;
    int is_watchdog;  /* TRUE if watchdog process, FALSE if user process */
} watchdog_data_t;

int SendPingSignal(void* args);
int CheckPingResponse(void* args);
void CleanupResources(scheduler_t* scheduler, char** argv, sem_t* wd_sem, sem_t* user_sem);
void HandleSignal(int sig);
int SetupSemaphores(sem_t** wd_sem, sem_t** user_sem, int is_watchdog);

extern atomic_int signal_flag;

#endif /* WD_COMMON_H */