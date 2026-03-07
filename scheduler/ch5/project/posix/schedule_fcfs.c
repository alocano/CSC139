/************************************************************/
/*First Come, First Served (FCFS) Scheduling Algorithm      */
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

void add(char *name, int priority, int burst){
    Task *newTask = malloc(sizeof(Task));
    newTask -> name = malloc(sizeof(Task));
    strcpy(newTask -> name, name);
    newTask -> tid = task_count++;
    newTask -> priority = priority;
    newTask -> burst = burst;

    insert(&head, newTask);
}

void schedule(){
    printf("=== FCFS Scheduling ===\n\n");

    struct node *temp = head;
    
    struct node *prev = NULL;
    struct node *current = head;
    struct node *next = NULL;

    while(current != NULL){
        next = current -> next;
        current -> next = prev;
        prev = current;
        current = next;
    }
    head = prev;

    temp = head;
    task_count = 0;

    printf("Task\tPriority\tBurst\tStart\tCompletion\tTurnaround\tWaiting\n");
    printf("================================================================================================\n");

    while(temp != NULL){
        Task *task = temp -> task;
        int start_time = current_time;
        int completion_time = current_time + task -> burst;
        int turnaround_time = completion_time - start_time;
        int waiting_time = start_time;
        int response_time = start_time;

        run(task, task -> burst);

        printf("%-10s\t%-10d\t%-5d\t%-5d\t%-10d\t%-10d\t%-10d\n", 
               task -> name, task -> priority, task -> burst, start_time, completion_time, turnaround_time, waiting_time);
        
        total_turnaround_time += turnaround_time;
        total_waiting_time += waiting_time;
        total_response_time += response_time;
        task_count++;

        current_time = completion_time;
        temp = temp -> next;
    }
    while (temp != NULL) {
    struct node *next = temp->next;
    free(temp->task->name);  // Free name
    free(temp->task);        // Free task
    free(temp);              // Free node
    temp = next;
}
    printf("\nAverage Turnaround Time: %.2f\n", (float)total_turnaround_time / task_count);
    printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / task_count);
    printf("Average Response Time: %.2f\n", (float)total_response_time / task_count);
}
