// Althaea Locano CSC 139 HW2.4 wzip.c
// Replicate the Unix zip command in C.

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    // Check if file is specified
    if(argc < 2){
        printf("Usage: wzip file1 ... \n");
        exit(1);
    }
    int count = 0;
    char curr_char = '\0';
    int first_char = 1; // Flag to indicate the first character

    for(int i = 1; i < argc; i++){
        FILE *fp = fopen(argv[i], "r");
        if(fp == NULL){
            printf("wzip: cannot open file\n");
            exit(1);
        }
        int c;
        while((c = fgetc(fp)) != EOF){
            if(first_char){
                curr_char = (char)c;
                count = 1;
                first_char = 0;
            }
            else if((char)c == curr_char){
                count++;
            }
            else {
                // Different character, write out the previous run
                fwrite(&count, sizeof(int), 1, stdout);
                fwrite(&curr_char, sizeof(char), 1, stdout);
                
                // Start new run
                curr_char = (char)c;
                count = 1;
            }
        }
        
        fclose(fp);
    }
    if (!first_char) {
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&curr_char, sizeof(char), 1, stdout);
    }
    exit(0);
}