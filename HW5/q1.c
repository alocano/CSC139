/******************************************************************/
/*Name: Althaea Locano                                            */
/*Date: 2/24/2026                                                 */
/*Assignment: HW5, Problem 1: Write a program that calls fork()...*/
/******************************************************************/  

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    int x = 100; //"have the main process access a variable (e.g., x) and set its value to something (e.g., 100)."
    printf("[parent] before fork: x = %d  (pid=%d)\n", x, getpid());
    
    pid_t pid = fork();
    if (pid == 0) {
        //Child Process
        printf("[child]  just after fork: x = %d  (pid=%d)\n", x, getpid());
        x = 200;   //What happens to the variable when both the child and parent change the value of x? 
        printf("[child]  after setting x=200: x = %d\n", x);
        exit(0);
    }
    else if (pid > 0) {
        //Parent Process, observe whether or not child process's change affects the parent
        wait(NULL); // Wait for the child process to finish
        printf("[parent] after child exits: x = %d\n", x);
        x = 300;
        printf("[parent] after setting x=300: x = %d\n", x);
    }
    else{
        fprintf(stderr, "Fork failed\n");
        exit(1);
    }
    exit(0);
}
