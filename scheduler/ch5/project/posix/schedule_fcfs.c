/************************************************************/
/*First Come, First Served (FCFS) Scheduling Algorithm      */
/*Althaea Locano CSC 139-01                                 */
/*March 5, 2026                                             */
/************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "schedulers.h"
#include "cpu.h"
#include "task.h"
#include "list.h"

struct node *head = NULL;

int turnaround_time = 0;
int waiting_time = 0;
int response_time = 0;
int total_tasks = 0;
int current_time = 0;

void add(char *name, int priority, int burst){
    Task *new_task = malloc(sizeof(Task));
    new_task -> name = malloc(strlen(name) + 1);
}