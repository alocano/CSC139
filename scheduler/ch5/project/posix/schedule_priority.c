/************************************************************/
/*Priority Scheduling Algorithm                             */
/*Althaea Locano CSC 139-01                                 */
/*March 5, 2026                                             */
/************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

// the list of tasks
struct node *head = NULL;

// initialize variables for times
int total_turnaround_time = 0;
int total_waiting_time = 0;
int total_response_time = 0;
int task_count = 0;
int current_time = 0;

// add a task to the list 
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = malloc(strlen(name) + 1);
    strcpy(newTask->name, name);
    newTask->tid = task_count++;
    newTask->priority = priority;
    newTask->burst = burst;
    
    insert(&head, newTask);
}

// Pick the task with the highest priority
Task *pickNextTask() {
    struct node *temp = head;
    struct node *highest = head;
    
    while (temp != NULL) {
        if (temp->task->priority > highest->task->priority) {
            highest = temp;
        }
        temp = temp->next;
    }
    
    return highest->task;
}

// invoke the scheduler
void schedule() {
    printf("=== Priority Scheduling ===\n\n");
    
    task_count = 0;
    
    printf("%-10s %-10s %-10s %-10s %-15s %-15s %-15s\n", 
           "Task", "Priority", "Burst", "Start", "Completion", "Turnaround", "Waiting");
    printf("================================================================================\n");
    
    while (head != NULL) {
        Task *task = pickNextTask();
        
        int start_time = current_time;
        int completion_time = current_time + task->burst;
        int turnaround_time = completion_time;  // arrival time is 0 for all
        int waiting_time = start_time;          // arrival time is 0 for all
        int response_time = start_time;         // same as waiting time for non-preemptive
        
        run(task, task->burst);
        
        printf("%-10s %-10d %-10d %-10d %-15d %-15d %-15d\n",
               task->name, task->priority, task->burst, start_time, 
               completion_time, turnaround_time, waiting_time);
        
        total_turnaround_time += turnaround_time;
        total_waiting_time += waiting_time;
        total_response_time += response_time;
        task_count++;
        
        current_time = completion_time;
        
        // remove completed task from list & free allocated memory
        delete(&head, task);
        free(task->name);
        free(task);
    }
    printf("\nAverage Turnaround Time: %.2f\n", (float)total_turnaround_time / task_count);
    printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / task_count);
    printf("Average Response Time: %.2f\n", (float)total_response_time / task_count);
}