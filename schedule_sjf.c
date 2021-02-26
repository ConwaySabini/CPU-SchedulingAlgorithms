/*
* The schedule_sjf file contains the shortest job first scheduler works
* by always executing the task with the lowest burst time. 
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

// pick shortest task
bool comesBefore(int a, int b)
{
    if (a <= b)
        return true;
    else
        return false;
}

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

    while (temp != NULL)
    {
        if (comesBefore(temp->task->burst, best_sofar->burst))
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
        run(task, task->burst);
        current_time += task->burst;
        printf("%s", "        Time is now: ");
        printf("%d\r\n", current_time);
        free(task->name);
        free(task);
        task = pickNextTask();
    }
    printf("\n");
}