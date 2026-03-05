/******************************************************************/
/*Name: Althaea Locano                                            */
/*Date: 2/24/2026                                                 */
/*Assignment: HW5, Problem 6: Write a slight modification of the  */
/*previous program, this time using waitpid() instead of wait()...*/
/******************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); exit(1); }
    if (pid == 0) {
        pid_t cWait = waitpid(-1, NULL, 0);
        printf("I am child process, my PID is %d\n", getpid());
        printf("Return code from waitpid() is %d\n", cWait);
    } else if(pid > 0){
        printf("I am parent process, my PID is %d\n", getpid());

    }
    else{
        fprintf(stderr, "Fork failed\n");
        exit(1);
    }
    exit(0);
}