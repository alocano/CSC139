/************************************************************/
/*Round Robin (RR) Scheduling Algorithm                     */
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

// Structure to track task metrics
typedef struct {
    char *name;
    int response_time;
    int waiting_time;
    int turnaround_time;
    int first_run;
} TaskMetrics;

TaskMetrics metrics[100];
int metrics_count = 0;

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

void schedule() {
    printf("=== Round Robin Scheduling (Time Quantum = %d) ===\n\n", QUANTUM);
    
    // Reverse the list to maintain arrival order
    struct node *prev = NULL;
    struct node *current = head;
    struct node *next = NULL;
    
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head = prev;
    
    task_count = 0;
    
    // Initialize metrics
    struct node *temp = head;
    while (temp != NULL) {
        metrics[metrics_count].name = temp->task->name;
        metrics[metrics_count].first_run = -1;
        metrics[metrics_count].waiting_time = 0;
        metrics_count++;
        temp = temp->next;
    }
   
    printf("%-10s %-10s %-10s %-10s %-15s %-15s %-15s\n", 
           "Task", "Priority", "Burst", "Start", "Completion", "Turnaround", "Waiting");
    printf("================================================================================\n");
    
    int finished = 0;
    while (!finished) {
        finished = 1;
        temp = head;
        
        while (temp != NULL) {
            Task *task = temp->task;
            
            if (task->burst > 0) {
                finished = 0;
                
                // Find metrics for this task
                int metric_idx = -1;
                for (int i = 0; i < metrics_count; i++) {
                    if (strcmp(metrics[i].name, task->name) == 0) {
                        metric_idx = i;
                        break;
                    }
                }
                
                // Record response time on first run
                if (metrics[metric_idx].first_run == -1) {
                    metrics[metric_idx].first_run = current_time;
                }
                
                int slice = (task->burst > QUANTUM) ? QUANTUM : task->burst;
                run(task, slice);
                
                printf("%-15s %-10d %-10d %-15d\n", 
                       task->name, slice, task->burst - slice, current_time);
                
                task->burst -= slice;
                current_time += slice;
                
                // If task is done, record completion time
                if (task->burst == 0) {
                    metrics[metric_idx].turnaround_time = current_time;
                }
            }
            
            temp = temp->next;
        }
    }
    
    printf("\n");
    printf("%-10s %-10s %-15s %-15s %-15s\n", 
           "Task", "Priority", "Response", "Turnaround", "Waiting");
    printf("================================================================================\n");
    
    for (int i = 0; i < metrics_count; i++) {
        int response = metrics[i].first_run;
        int turnaround = metrics[i].turnaround_time;
        int waiting = turnaround - response - QUANTUM; // Approximate waiting
        if (waiting < 0) waiting = 0;
        
        // Find priority for this task
        temp = head;
        int priority = 0;
        while (temp != NULL) {
            if (strcmp(temp->task->name, metrics[i].name) == 0) {
                priority = temp->task->priority;
                break;
            }
            temp = temp->next;
        }
        
        waiting = turnaround - response;
        
        printf("%-10s %-10d %-15d %-15d %-15d\n",
               metrics[i].name, priority, response, turnaround, waiting);
        
        total_response_time += response;
        total_turnaround_time += turnaround;
        total_waiting_time += waiting;
    }
    
    printf("\nAverage Response Time: %.2f\n", (float)total_response_time / metrics_count);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / metrics_count);
    printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / metrics_count);
}