/******************************************************************/
/*Name: Althaea Locano                                            */
/*Date: 2/24/2026                                                 */
/*Assignment: HW5, Problem 4: Write a program that calls fork()   */
/*and then calls some form of exec() to run the program /bin/ls   */
/******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); exit(1); }

    if (pid == 0) { 
        char * const argv[] = {"ls", NULL};
        char * const envp[] = {NULL};
        
        //==== EXECL ====
        //execl("/bin/ls", "ls", NULL);

        //==== EXECLP ====
        //execlp("ls", "ls", NULL);

        //==== EXECLE ====
        execle("/bin/ls", "ls", NULL, envp);
        
        //==== EXECV ====
        //execv("/bin/ls", argv);
        
        //==== EXECVP ====
        //execvp("ls", argv);
        exit(0);
    } else {
        //Parent Process
        wait(NULL); // Wait for the child process to finish
        printf("Child process finished\n");
    }
    exit(0);
}