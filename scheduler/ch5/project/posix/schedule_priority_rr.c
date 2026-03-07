/************************************************************/
/*Priority with Round Robin Algorithm                       */
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

struct node *head = NULL;

int total_turnaround_time = 0;
int total_waiting_time = 0;
int total_response_time = 0;
int task_count = 0;
int current_time = 0;

typedef struct {
    char *name;
    int response_time;
    int waiting_time;
    int turnaround_time;
    int first_run;
    int original_burst;
} TaskMetrics;

TaskMetrics metrics[100];
int metrics_count = 0;

void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = malloc(strlen(name) + 1);
    strcpy(newTask->name, name);
    newTask->tid = task_count++;
    newTask->priority = priority;
    newTask->burst = burst;
    
    metrics[metrics_count].name = malloc(strlen(name) + 1);
    strcpy(metrics[metrics_count].name, name);
    metrics[metrics_count].first_run = -1;
    metrics[metrics_count].original_burst = burst;
    metrics_count++;
    
    insert(&head, newTask);
}

int getHighestPriority() {
    struct node *temp = head;
    int highest = -1;
    
    while (temp != NULL) {
        if (temp->task->burst > 0 && temp->task->priority > highest) {
            highest = temp->task->priority;
        }
        temp = temp->next;
    }
    
    return highest;
}

void schedule() {
    printf("Priority with Round Robin Scheduling (Time Quantum = %d)\n\n", QUANTUM);
    
    task_count = 0;
    
    printf("%-15s %-10s %-10s %-10s %-15s\n", 
           "Task", "Priority", "Burst", "Remaining", "Time");
    printf("=========================================================\n");
    
    int finished = 0;
    while (!finished) {
        finished = 1;
        
        // get highest priority among remaining tasks
        int highest_priority = getHighestPriority();
        
        if (highest_priority == -1) {
            break;  // no tasks left
        }
        
        // process all tasks at this priority level in round-robin fashion
        struct node *temp = head;
        int found_task = 0;
        
        while (temp != NULL) {
            Task *task = temp->task;
            
            if (task->burst > 0 && task->priority == highest_priority) {
                finished = 0;
                found_task = 1;
                
                int metric_idx = -1;
                for (int i = 0; i < metrics_count; i++) {
                    if (strcmp(metrics[i].name, task->name) == 0) {
                        metric_idx = i;
                        break;
                    }
                }
                
                // record response time on first run
                if (metrics[metric_idx].first_run == -1) {
                    metrics[metric_idx].first_run = current_time;
                }
                
                int slice = (task->burst > QUANTUM) ? QUANTUM : task->burst;
                run(task, slice);
                
                printf("%-15s %-10d %-10d %-10d %-15d\n", 
                       task->name, task->priority, slice, task->burst - slice, current_time);
                
                task->burst -= slice;
                current_time += slice;
                
                // If task is done, record completion time
                if (task->burst == 0) {
                    metrics[metric_idx].turnaround_time = current_time;
                }
            }
            
            temp = temp->next;
        }
        
        if (!found_task) {
            finished = 1;
        }
    }
    
    printf("\n");
    printf("%-10s %-10s %-15s %-15s %-15s\n", 
           "Task", "Priority", "Response", "Turnaround", "Waiting");
    printf("==================================================================\n");
    
    for (int i = 0; i < metrics_count; i++) {
        int response = metrics[i].first_run;
        int turnaround = metrics[i].turnaround_time;
        int waiting = turnaround - metrics[i].original_burst;
        
        printf("%-10s %-10d %-15d %-15d %-15d\n",
               metrics[i].name, 
               (i < metrics_count ? 0 : 0),  // We'd need to look this up
               response, turnaround, waiting);
        
        total_response_time += response;
        total_turnaround_time += turnaround;
        total_waiting_time += waiting;
    }

    printf("\nAverage Response Time: %.2f\n", (float)total_response_time / metrics_count);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / metrics_count);
    printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / metrics_count);
}