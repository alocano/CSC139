#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Check if at least one file is provided
    if (argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }
    
    int count = 0;
    char current_char = '\0';
    int first_char = 1;
    
    // Process each file
    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        
        if (fp == NULL) {
            printf("wzip: cannot open file\n");
            exit(1);
        }
        
        int c;
        while ((c = fgetc(fp)) != EOF) {
            if (first_char) {
                // First character we've seen
                current_char = (char)c;
                count = 1;
                first_char = 0;
            } else if ((char)c == current_char) {
                // Same character, increment count
                count++;
            } else {
                // Different character, write out the previous run
                fwrite(&count, sizeof(int), 1, stdout);
                fwrite(&current_char, sizeof(char), 1, stdout);
                
                // Start new run
                current_char = (char)c;
                count = 1;
            }
        }
        
        fclose(fp);
    }
    
    // Write out the last run (if any)
    if (!first_char) {
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&current_char, sizeof(char), 1, stdout);
    }
    
    return 0;
}