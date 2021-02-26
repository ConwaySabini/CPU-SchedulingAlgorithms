/*
* The schedule_rr file contains the round robin scheduler that operates using
* a time quantum of 10 defined in the cpu.h. The round robin scheduler works
* by running each task in the list until it is finished or has reached the 
* end of the time quantum at which point it will switch to the next task.
*
* @author Sabini Ethan
* @date 2/7/2021
* @version 9.3.0
*/
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"
#include "cpu.h"
#include "task.h"

struct node *g_head = NULL;

// adds a task in the list w/ burst time
void add(char *name, int priority, int burst)
{
    Task *task = malloc(sizeof(Task));
    // higher number is a higher priority
    task->priority = priority;
    task->burst = burst;
    task->name = strdup(name);
    insert(&g_head, task);
    free(name);
}

// based on traverse from list.c
// finds the task whose name comes first in dictionary
Task *pickNextTask()
{
    // if list is empty, nothing to do
    if (!g_head)
        return NULL;

    struct node *temp;
    temp = g_head;
    Task *best_sofar = temp->task;

    // iterate to the end of the list
    while (temp != NULL)
    {
        best_sofar = temp->task;
        temp = temp->next;
    }
    // delete the node from list, Task will get deleted later
    delete (&g_head, best_sofar);
    return best_sofar;
}

// schedules and runs every task in the list
void schedule()
{
    int current_time = 0;
    Task *task = pickNextTask();
    while (task)
    {
        int remaining = 0;          // remaining time
        if (task->burst <= QUANTUM) // execute whole task
        {
            run(task, task->burst);
            current_time += task->burst;
            printf("%s", "        Time is now: ");
            printf("%d\r\n", current_time);
            free(task->name);
            free(task);
        }
        else // execute part of task and insert in list
        {
            remaining = task->burst - QUANTUM;
            run(task, QUANTUM);
            current_time += QUANTUM;
            printf("%s", "        Time is now: ");
            printf("%d\r\n", current_time);
            task->burst = remaining;
            insert(&g_head, task);
        }
        task = pickNextTask();
    }
    printf("\n");
}