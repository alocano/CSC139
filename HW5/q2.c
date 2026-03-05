/******************************************************************/
/*Name: Althaea Locano                                            */
/*Date: 2/24/2026                                                 */
/*Assignment: HW5, Problem 2: Write a program that opens a file   */
/*(with the open() system call)...                                */
/******************************************************************/ 

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(){
    //" opens a file (with the open() system call)... "
    FILE *my_file = fopen("q2_output.txt", "w");
    if (my_file == NULL) {
        perror("fopen");
        exit(1);
    }
    printf("[parent] opened file, file descriptor = %d\n", fileno(my_file));

    //"...and then calls fork() to create a new process"
    fflush(my_file); // Ensure the file is flushed before forking
    pid_t pid = fork();

    if (pid < 0) { 
        perror("fork"); 
        exit(1); 
    }

    if (pid == 0) {
        const char *child_msg = "This is the child process writing to the file.\n";
        printf("Child accessing File Descriptor (file handle): %d\n", fileno(my_file));
        fprintf(my_file, "%s", child_msg);
    } 
    else if(pid > 0){
        const char *parent_msg = "This is the parent process writing to the file.\n";
        printf("Parent accessing File Descriptor (file handle): %d\n", fileno(my_file));
        fprintf(my_file, "%s", parent_msg);
    }
    printf("\nFile %d closed\n", fileno(my_file));
    fclose(my_file);
    exit(0);
}
