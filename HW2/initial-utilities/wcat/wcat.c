#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Check if file specified, if not, exit 
    if (argc == 1) {
        exit(0);
    }
    
    // Process each file
    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        
        // Check if file opened successfully
        if (fp == NULL) {
            printf("wcat: cannot open file\n");
            exit(1);
        }
        
        // Read and print file contents line by line
        char buffer[4096];
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            printf("%s", buffer);
        }
        
        // Close the file
        fclose(fp);
    }
    
    exit(0);
}