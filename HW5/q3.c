/******************************************************************/
/*Name: Althaea Locano                                            */
/*Date: 2/24/2026                                                 */
/*Assignment: HW5, Problem 3: Write another program using fork(). */
/*The child process should print “hello”;...                      */
/******************************************************************/ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int pipefd[2];
    //Error Checking for pipe() and fork()
    if (pipe(pipefd) < 0) { 
        perror("pipe"); 
        exit(1); 
    }
    pid_t pid = fork();
    if (pid < 0) { 
        perror("fork"); 
        exit(1); 
    }

    if (pid == 0) {
        close(pipefd[0]);           // close read end — child only writes
        printf("hello\n");
        fflush(stdout);
        write(pipefd[1], "x", 1);   //signal parent we are done
        close(pipefd[1]);
        exit(0);
    } else if(pid > 0){
        //Parent Process
        close(pipefd[1]);           // close write end — parent only reads
        char buff[1];
        read(pipefd[0], buff, 1);    // BLOCK until child signals
        close(pipefd[0]);
        printf("goodbye\n");
    }
    exit(0);
}