/*********************************************/
/*Althaea Locano 2-10-26 HW3: Reverse Program*/
/*Print the reverse contents of a file       */
/*********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

struct node {
    char *line;
    struct node *next;
};

// Compare 
int same_file(const char *file1, const char *file2) {
    struct stat stat1, stat2;
    
    if (stat(file1, &stat1) != 0) {
        return 0;
    }
    if (stat(file2, &stat2) != 0) {
        return 0;
    }
    
    return (stat1.st_ino == stat2.st_ino && stat1.st_dev == stat2.st_dev);
}

int main(int argc, char *argv[]) {
    FILE *input = stdin;
    FILE *output = stdout;
    char buffer[4096];
    struct node *head = NULL;
    
    //Error check for too many arguments, same file, and file opening issues
    //If there are too many arguments, print usage and exit with error code
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }
    
    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }
    
    if (argc == 3) {
        if (same_file(argv[1], argv[2])) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            if (input != stdin) fclose(input);
            exit(1);
        }
        
        output = fopen(argv[2], "w");
        if (output == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
            if (input != stdin) fclose(input);
            exit(1);
        }
    }
    
    while (fgets(buffer, sizeof(buffer), input) != NULL) {
        struct node *new_node = malloc(sizeof(struct node));
        if (new_node == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        
        size_t len = strlen(buffer);
        
        // Add new line if needed
        int needs_newline = (len > 0 && buffer[len - 1] != '\n');
        
        new_node->line = malloc(len + needs_newline + 1);
        if (new_node->line == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        
        //Copy line into node
        memcpy(new_node->line, buffer, len);
        
        //Add new line if needed
        if (needs_newline) {
            new_node->line[len] = '\n';
            new_node->line[len + 1] = '\0';
        } else {
            new_node->line[len] = '\0';
        }
        
        new_node->next = head;
        head = new_node;
    }
    
    //Print lines and free memory prev. allocated
    while (head != NULL) {
        struct node *temp = head;
        fprintf(output, "%s", head->line);
        head = head->next;
        free(temp->line);
        free(temp);
    }
    
    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);
    
    exit(0);
}