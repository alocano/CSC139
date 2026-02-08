//Althaea Locano CSC 139 HW2.1 wcat.c
// Replicate the Unix cat command in C.

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    //Check if at least one file specified
    if(argc < 2){
        printf("Usage: wcat <file> ...\n");
        exit(1);
    }
    //Loop through each file and print its contents
    for(int i = 1; i < argc; i++){
        FILE *fp = fopen(argv[i], "r");
        if(fp == NULL){
            printf("Error: Could not open file\n");
            exit(1);
        }
        else{
            char buffer[1024];
            while(fgets(buffer, sizeof(buffer), fp) != NULL){
                printf("%s", buffer);
        }
        fclose(fp);
        }
        exit(0);
    }
}