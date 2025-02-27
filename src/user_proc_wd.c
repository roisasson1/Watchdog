#define _GNU_SOURCE
#include <stdio.h>    /* printf, fprintf */
#include <stdlib.h>   /* atoi */
#include <unistd.h>   /* execvp */
#include <signal.h>   /* sigaction, kill, SIGUSR1, SIGUSR2 */
#include <pthread.h>  /* pthread_create, pthread_exit */

#include "scheduler.h" /* scheduler API */
#include "wd_common.h" /* shared objects API */

static sem_t* wd_sem_local = NULL;
static sem_t* user_sem_local = NULL;

void WDProcess(char** argv);
void WDSigStopHandler(int sig);

int main(int argc, char** argv)
{
    (void)argc;
    WDProcess(argv);
    return 0;
}

void WDProcess(char** argv)
{
    watchdog_data_t watchdog = {0};
    struct sigaction wd_stop = {0};
    struct sigaction wd = {0};

    /* setup watchdog data */
    watchdog.args = argv;
    watchdog.is_watchdog = TRUE;
    watchdog.interval = atoi(argv[1]);
    watchdog.tolerance = atoi(argv[2]);

    /* setup signal handlers - signal handler and stopWD handler */
    wd.sa_handler = HandleSignal;
    wd_stop.sa_handler = WDSigStopHandler;
    sigaction(SIGUSR1, &wd, NULL);
    sigaction(SIGUSR2, &wd_stop, NULL);

    if (0 != SetupSemaphores(&wd_sem_local, &user_sem_local, TRUE))
    {
        fprintf(stderr, "[Watchdog] Failed to setup semaphores\n");
        return;
    }

    /* the wd scheduler needs to wait for the user process scheduler */
    sem_post(user_sem_local);
    sem_wait(wd_sem_local);

    watchdog.scheduler = SchedulerCreate();
    if (NULL == watchdog.scheduler)
    {
        CleanupResources(NULL, NULL, wd_sem_local, user_sem_local);
        return;
    }

    /* add monitoring tasks */
    SchedulerAddTask(watchdog.scheduler, SendPingSignal, &watchdog, 1, NULL, NULL);
    SchedulerAddTask(watchdog.scheduler, CheckPingResponse, &watchdog, 2, NULL, NULL);

    if (STOP == SchedulerRun(watchdog.scheduler))
    {
        CleanupResources(watchdog.scheduler, NULL, wd_sem_local, user_sem_local);
        printf("[Watchdog] User process is unresponsive. Restarting user process...\n");
        execvp(USER_PROCESS, argv);
    }
}

void WDSigStopHandler(int sig)
{
    (void)sig;
    printf("[Watchdog] Received stop signal (SIGUSR2). Cleaning up resources...\n");
    CleanupResources(NULL, NULL, wd_sem_local, user_sem_local);
    raise(SIGKILL);
}