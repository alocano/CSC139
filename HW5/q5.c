/******************************************************************/
/*Name: Althaea Locano                                            */
/*Date: 2/24/2026                                                 */
/*Assignment: HW5, Problem 5: Now write a program that uses       */
/*wait() to wait for the child process to finish in the parent.   */
/******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); exit(1); }
    if (pid == 0) {
        pid_t cWait = wait(NULL);
         printf("I am child process, my PID is %d\n", getpid());
        printf("Return code from wait() is %d\n", cWait);
    } else if(pid > 0){
        printf("I am parent process, my PID is %d\n", getpid());

    }
    else{
        fprintf(stderr, "Fork failed\n");
        exit(1);
    }
    exit(0);
}