#define _GNU_SOURCE
#include <stdio.h>     /* printf */
#include <stdlib.h>    /* malloc, getenv, atoi, setenv */
#include <string.h>    /* strdup */
#include <assert.h>    /* assert */
#include <unistd.h>    /* fork, execvp, getpid, getppid */
#include <signal.h>    /* sigaction, kill, SIGUSR1, SIGUSR2 */
#include <pthread.h>   /* pthread_create, pthread_exit */

#include "wd.h"        /* API definitions */
#include "scheduler.h" /* scheduler API */
#include "wd_common.h" /* shared objects API */

typedef struct
{
    pthread_t monitor_thread;
    watchdog_data_t data;
} watchdog_process_t;

static watchdog_process_t wd_g = {0}; /* global wd for cleanup func */
static sem_t* wd_sem_g = NULL;
static sem_t* user_sem_g = NULL;

static void* UserScheduler(void* args);
static char** GenerateArgs(int argc, char** argv, size_t interval, unsigned int tolerance);

wd_status_t WDStart(int argc, const char* argv[], size_t interval, unsigned int tolerance)
{
    pid_t pid;
    char buffer_g[BUFFER_LEN];
    struct sigaction user = {0};

    assert(NULL != argv);

    /* setup watchdog data */
    wd_g.data.interval = interval;
    wd_g.data.tolerance = tolerance;
    wd_g.data.args = GenerateArgs(argc, (char**)argv, interval, tolerance);
    wd_g.data.is_watchdog = FALSE;

    user.sa_handler = HandleSignal;
    sigaction(SIGUSR1, &user, NULL);

    if (0 != SetupSemaphores(&wd_sem_g, &user_sem_g, FALSE))
    {
        return SEM_OPEN_FAILED;
    }

    /* create WD daemon process */
    pid = fork();
    if (pid < 0)
    {
        return FORK_FAILED;
    }
    else if (0 == pid)
    {
        /* child process - execute wd process */
        execvp(WD_PROCESS, wd_g.data.args);

        /* if exec fails - send parent SIGUSR2 */
        kill(getppid(), SIGUSR2);
        raise(SIGKILL);
    }
    else
    {
        /* parent process */
        sprintf(buffer_g, "%d", pid);
        setenv(PID_ENV, buffer_g, TRUE);

        sem_post(wd_sem_g);
        sem_wait(user_sem_g);

        /* create thread for monitoring */
        if (0 != pthread_create(&wd_g.monitor_thread, NULL, UserScheduler, &wd_g.data))
        {
            CleanupResources(wd_g.data.scheduler, wd_g.data.args, wd_sem_g, user_sem_g);
            return THREAD_CREATION_FAILED;
        }
    }

    return SUCCESS;
}

void WDStop()
{
    pid_t pid = 0;
    char* pid_str = getenv(PID_ENV);

    CleanupResources(wd_g.data.scheduler, wd_g.data.args, wd_sem_g, user_sem_g);
    pthread_detach(wd_g.monitor_thread);
    pid = atoi(pid_str);
    kill(pid, SIGUSR2);
}

static void* UserScheduler(void* args)
{
    watchdog_data_t* data = (watchdog_data_t*)args;
    pid_t pid;
    char buffer_g[BUFFER_LEN];
    
    data->scheduler = SchedulerCreate();
    if (NULL == data->scheduler)
    {
        CleanupResources(NULL, data->args, wd_sem_g, user_sem_g);
        return NULL;
    }

    /* Add tasks for monitoring */
    SchedulerAddTask(data->scheduler, SendPingSignal, data, 1, NULL, NULL);
    SchedulerAddTask(data->scheduler, CheckPingResponse, data, 3, NULL, NULL);

    /* While wd is dead - revive wd */
    while (STOP == SchedulerRun(data->scheduler))
    {
        pid = fork();
        if (pid < 0)
        {
            return NULL;
        }
        else if (0 == pid)
        {
            execvp(WD_PROCESS, data->args);
            kill(getppid(), SIGUSR2);
            raise(SIGKILL);
        }
        else
        {
            sprintf(buffer_g, "%d", pid);
            setenv(PID_ENV, buffer_g, TRUE);

            sem_post(wd_sem_g);
            sem_wait(user_sem_g);

            SchedulerClear(data->scheduler);
            SchedulerAddTask(data->scheduler, SendPingSignal, data, 1, NULL, NULL);
            SchedulerAddTask(data->scheduler, CheckPingResponse, data, 2, NULL, NULL);
        }
    }

    pthread_exit(NULL);
    return args;
}

static char** GenerateArgs(int argc, char** argv, size_t interval, unsigned int tolerance)
{
    char** returned_args = (char**)malloc((argc + 4) * sizeof(char*));
    char interval_str[MAX_PID_DIGITS];
    char tolerance_str[MAX_PID_DIGITS];
    int i = 0;
    int j = 0;

    if (NULL == returned_args)
    {
        return NULL;
    }

    returned_args[0] = strdup(WD_PROCESS);
    sprintf(interval_str, "%ld", interval);
    sprintf(tolerance_str, "%d", tolerance);
    returned_args[1] = strdup(interval_str);
    returned_args[2] = strdup(tolerance_str);

    i = 3;
    while (j < argc)
    {
        returned_args[i] = strdup(argv[j]);
        ++i;
        ++j;
    }

    returned_args[i] = NULL;
    return returned_args;
}