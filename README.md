# Watchdog Implementation

This project implements a watchdog system designed to monitor and revive a user process if it becomes unresponsive.

## Overview

The watchdog system consists of two main processes: the user process and the watchdog process. 
The user process performs the main application logic, while the watchdog process monitors its health. 
The watchdog sends ping signals to the user process and checks for timely responses. 
If the user process fails to respond within a specified tolerance, the watchdog restarts it.

## Compilation

To compile the project, use the following commands:
1. compile user process:
gd wd_process.out src/scheduler.c src/user_proc_wd.c src/wd_common.c ../scheduler/src/task.c ../../ds/src/pqueue.c ../../ds/src/heap.c ../../ds/src/vector.c ../../ds/src/sdll.c ../../ds/src/dll.c  ../scheduler/src/uid.c -Iinclude

2. compile watchdog process:
gd user_wd.out src/wd.c test/test_wd.c src/wd_common.c scheduler/src/scheduler.c scheduler/src/task.c scheduler/src/pqueue.c scheduler/src/heap.c scheduler/src/vector.c scheduler/src/sdll.c scheduler/src/dll.c  scheduler/src/uid.c -Iinclude

3. run:
./user_wd.out
