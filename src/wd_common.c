#define _GNU_SOURCE
#include <stdio.h>     /* printf */
#include <stdlib.h>    /* getenv, atoi */
#include <string.h>    /* strcpy */
#include <signal.h>    /* kill, SIGUSR1 */
#include <fcntl.h>     /* O_CREAT */
#include <sys/stat.h>  /* S_IRUSR, S_IWUSR */
#include <time.h>      /* time */

#include "wd_common.h" /* shared objects API */

#define WATCHDOG "Watchdog"
#define USER "User"

atomic_int signal_flag = FALSE;

int SendPingSignal(void* args)
{
    pid_t target_pid;
    watchdog_data_t* data = (watchdog_data_t*)args;
    char process_name[BUFFER_LEN];
    char target_str[BUFFER_LEN];

    strcpy(process_name, data->is_watchdog ? WATCHDOG : USER);
    strcpy(target_str, process_name);

    if (data->is_watchdog) 
    {
        target_pid = getppid(); /* Watchdog sends to parent (user process) */
    } 
    else 
    {
        char* pid_str = getenv(PID_ENV);
        target_pid = atoi(pid_str);
    }

    printf("[%s] Sending ping signal to %s (PID: %d)\n",
           process_name, target_str, target_pid);

    kill(target_pid, SIGUSR1); /* send signal to the other process */
    return CONTINUE;
}

int CheckPingResponse(void* args)
{
    watchdog_data_t* data = (watchdog_data_t*)args;
    time_t start = time(NULL);
    time_t updated_time = 0;
    int tolerance = data->tolerance;
    char process_name[BUFFER_LEN];
    char target_str[BUFFER_LEN];

    strcpy(process_name, data->is_watchdog ? WATCHDOG : USER);
    strcpy(target_str, process_name);

    printf("[%s] Starting ping response check (Interval: %lu, Tolerance: %d)\n",
           process_name, data->interval, tolerance);

    /* while tolerance did not exceeded - check if got signal in time */
    do
    {
        updated_time = time(NULL);
        /* if got signal - break and continue code */
        if (signal_flag == TRUE)
        {
            atomic_store(&signal_flag, FALSE);
            printf("[%s] Received ping response from %s\n", process_name, target_str);
            break;
        }
        /* if didn't get signal in time - try again */
        else if (difftime(updated_time, start) >= (double)data->interval)
        {
            atomic_fetch_sub(&tolerance, 1);
            start = time(NULL);
            printf("[%s] No response from %s. Remaining tolerance: %d\n", 
                   process_name, target_str, tolerance);
        }
    } while (tolerance > 0);

    if (tolerance <= 0)
    {
        printf("[%s] %s is unresponsive. Stopping scheduler...\n", process_name, target_str);
        SchedulerStop(data->scheduler);
        return SUCCESS;
    }

    return CONTINUE;
}

void CleanupResources(scheduler_t* scheduler, char** argv, sem_t* wd_sem, sem_t* user_sem)
{
    size_t i = 0;

    if (NULL != scheduler)
    {
        SchedulerDestroy(scheduler);
    }

    if (NULL != argv)
    {
        while (NULL != argv[i])
        {
            free(argv[i]);
            ++i;
        }
        free(argv);
    }

    if (NULL != wd_sem)
    {
        sem_close(wd_sem);
        sem_unlink(WD_SEM);
        sem_destroy(wd_sem);
    }

    if (NULL != user_sem)
    {
        sem_close(user_sem);
        sem_unlink(USER_SEM);
        sem_destroy(user_sem);
    }
}

void HandleSignal(int sig)
{
    (void)sig;
    atomic_store(&signal_flag, TRUE);
}

int SetupSemaphores(sem_t** wd_sem, sem_t** user_sem, int is_watchdog)
{
    if (is_watchdog)
    {
        /* watchdog process opens existing semaphores */
        *wd_sem = sem_open(WD_SEM, O_RDWR);
        *user_sem = sem_open(USER_SEM, O_RDWR);
    }
    else
    {
        /* user process creates new semaphores */
        sem_unlink(WD_SEM);
        sem_unlink(USER_SEM);
        
        *wd_sem = sem_open(WD_SEM, O_CREAT, S_IRUSR | S_IWUSR, 0);
        *user_sem = sem_open(USER_SEM, O_CREAT, S_IRUSR | S_IWUSR, 0);
    }

    if (SEM_FAILED == *wd_sem || SEM_FAILED == *user_sem)
    {
        return FAIL;
    }

    return SUCCESS;
}