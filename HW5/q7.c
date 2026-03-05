/******************************************************************/
/*Name: Althaea Locano                                            */
/*Date: 2/24/2026                                                 */
/*Assignment: HW5, Problem 7: Write a program that creates a child*/
/*process, and then in the child closes standard output           */
/******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); exit(1); }

    if(pid == 0){
        close(STDOUT_FILENO);
        printf("Test\n");
    }
    else if(pid > 0){
        wait(NULL);
        printf("I am parent process, my PID is %d\n", getpid());
    }
}