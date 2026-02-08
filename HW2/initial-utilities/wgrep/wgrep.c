//Althaea Locano CSC 139 HW2.2 wgrep.c
// Replicate the Unix grep command in C.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void searchFile(FILE *fp, const char *searchTerm){
    char *line = NULL;
    size_t len = 0;
    
    while(getline(&line, &len, fp) != -1){
        if(strstr(line, searchTerm) != NULL){
            printf("%s", line);
        }
    }
    free(line);
}

int main(int argc, char *argv[]) {
    // Check if at least a search term is provided
    if (argc < 2) {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }
    
    const char *searchterm = argv[1];
    
    // If no files specified, read from stdin
    if (argc == 2) {
        searchFile(stdin, searchterm);
        return 0;
    }
    
    // Process each file
    for (int i = 2; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        
        // Check if file opened successfully
        if (fp == NULL) {
            printf("wgrep: cannot open file\n");
            exit(1);
        }
        
        searchFile(fp, searchterm);
        
        fclose(fp);
    }
    
    return 0;
}
