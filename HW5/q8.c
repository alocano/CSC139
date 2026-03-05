/******************************************************************/
/*Name: Althaea Locano                                            */
/*Date: 2/24/2026                                                 */
/*Assignment: HW5, Problem 8: Write a program that creates two    */
/*children, and connects the standard output of one to the        */
/*standard input of the other, using the pipe() system call.      */
/******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    int pipefd[2];
    if(pipe(pipefd) < 0){
        perror("pipe");
        exit(1);
    }

    // First Child Process
    pid_t pid1 = fork();
    if (pid1 < 0) { 
        perror("fork child1"); 
        exit(1); 
    }

    if (pid1 == 0) {
        
        close(pipefd[0]);                        
        dup2(pipefd[1], STDOUT_FILENO);          
        close(pipefd[1]);                        

        fprintf(stderr, "[child 1 pid=%d] starting: will run 'ls -1 .' and write output into pipe\n", getpid());
        execlp("ls", "ls", "-1", ".", (char*)NULL);
        perror("execlp ls");
        exit(1);
    }

    // Second Child Process
    pid_t pid2 = fork();
    if (pid2 < 0) { 
        perror("fork child2"); 
        exit(1); 
    }

    if (pid2 == 0) {
        
        close(pipefd[1]);                        
        dup2(pipefd[0], STDIN_FILENO);          
        close(pipefd[0]);                        

        fprintf(stderr, "[child 2 pid=%d] starting: will run 'wc -l' reading from pipe\n", getpid());
        fprintf(stderr, "[child 2 pid=%d] waiting for child 1's output...\n", getpid());
        execlp("wc", "wc", "-l", (char*)NULL);
        perror("execlp wc");
        exit(1);
    }

    // Final: Parent process
    close(pipefd[0]);  
    close(pipefd[1]);

    printf("[parent  pid=%d] spawned child 1 (pid=%d): ls -1 .\n", getpid(), pid1);
    printf("[parent  pid=%d] spawned child 2 (pid=%d): wc -l\n",   getpid(), pid2);
    printf("[parent  pid=%d] pipe connected: child 1 stdout → child 2 stdin\n", getpid());
    printf("[parent  pid=%d] line count from wc: ", getpid());
    fflush(stdout);

    waitpid(pid1, NULL, 0);
    fprintf(stderr, "[parent  pid=%d] child 1 (pid=%d) finished\n", getpid(), pid1);
    waitpid(pid2, NULL, 0);
    fprintf(stderr, "[parent  pid=%d] child 2 (pid=%d) finished\n", getpid(), pid2);

    printf("[parent  pid=%d] all done\n", getpid());
    return 0;
}
