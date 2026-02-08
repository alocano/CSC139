#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Check if at least one file is provided
    if (argc < 2) {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }
    
    // Process each file
    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        
        if (fp == NULL) {
            printf("wunzip: cannot open file\n");
            exit(1);
        }
        
        int count;
        char ch;
        
        // Read 4-byte integer and 1-byte character pairs
        while (fread(&count, sizeof(int), 1, fp) == 1) {
            if (fread(&ch, sizeof(char), 1, fp) == 1) {
                // Print the character 'count' times
                for (int j = 0; j < count; j++) {
                    printf("%c", ch);
                }
            }
        }
        
        fclose(fp);
    }
    
    return 0;
}