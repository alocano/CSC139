/*****************************************************/
/*Name: Althaea Locano CSC 139-01 Memory Management  */
/*Date: 26 Apr 2026                                  */
/*Implements FIFO, Least Recently Used (LRU),        */
/*and Optimal Page Replacement (OPT) algorithms.     */
/*Test the following reference string:               */
/*2,6,9,2,4,2,1,7,3,0,5,2,1,2,9,5,7,3,8,5            */
/*0,6,3,0,2,6,3,5,2,4,1,3,0,6,1,4,2,3,5,7            */
/*3,1,4,2,5,4,1,3,5,2,0,1,1,0,2,3,4,5,0,1            */
/*4,2,1,7,9,8,3,5,2,6,8,1,0,7,2,4,1,3,5,8            */
/*0,1,2,3,4,4,3,2,1,0,0,1,2,3,4,4,3,2,1,0            */
/*****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//define maximum amounts based on given instructions: 100 total pages
//max frames can start at 3 then go up to 5, then 10
//increasing the maximum for now to allow some wiggle room 
#define MAX_PAGES 100
#define MAX_FRAMES 20

static int in_frames(int *frames, int n, int page, int *pos){
    for(int i = 0; i < n; i++){
        if(frames[i] == page){
            if(pos){
                *pos = i;
                return 1;
            }
        }
    }
    return 0;
}

static void print_frames(int *frames, int n_frames) {
    printf("[");
    for (int i = 0; i < n_frames; i++) {
        if (frames[i] == -1) {
            printf(" -");
        } else {
            printf("%2d", frames[i]);
        }
        if (i < n_frames - 1) {
            printf(" ");
        }
    }
    printf("]");
}

//FIFO - Replace the page that has been in memory the longest; Memory treated like a queue
int fifo(int *pages, int n, int n_frames, int trace) {
    int frames[MAX_FRAMES];
    int faults = 0;
    int next_replace = 0;
 
    for (int i = 0; i < n_frames; i++) {
        frames[i] = -1;
    }
 
    if (trace) {
        printf("\n=== FIFO Trace (%d frames) ===\n", n_frames);
        printf("Step\tPage\tFrames\t\tFault?\tEvicted\n");
        printf("-----------------------------------------------------------\n");
    }
 
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int fault = 0;
        int evicted = -1;
 
        if (!in_frames(frames, n_frames, page, NULL)) {
            fault = 1;
            faults++;
            evicted = frames[next_replace];
            frames[next_replace] = page;
            next_replace = (next_replace + 1) % n_frames;
        }
 
        if (trace) {
            printf("%d\t %d", i + 1, page);
            print_frames(frames, n_frames);
            if (fault) {
                if (evicted == -1) {
                    printf("\tYes\n");
                } else {
                    printf("\tYes (%d removed)\n", evicted);
                }
            } else {
                printf("\tNo\n");
            }
            
        }
    }
    return faults;
}
 

//LRU - Replace the page that has not been accessed for the longest period of time; Assumes pages used recently are likely to be used again soon -> better than FIFO

int lru(int *pages, int n, int n_frames, int trace) {
    int frames[MAX_FRAMES];
    int last_used[MAX_FRAMES];
    int faults = 0;
 
    for (int i = 0; i < n_frames; i++) {
        frames[i]    = -1;
        last_used[i] = -1;
    }
 
    if (trace) {
        printf("\n=== LRU Trace (%d frames) ===\n", n_frames);
        printf("Step\tPage\tFrames\t\tFault?\tEvicted\n");
        printf("-----------------------------------------------------------\n");
    }
 
    for (int i = 0; i < n; i++) {
        int page    = pages[i];
        int fault   = 0;
        int evicted = -1;
        int pos     = -1;
 
        if (in_frames(frames, n_frames, page, &pos)) {
            last_used[pos] = i;
        } else {
            fault = 1;
            faults++;
 
            //Look for an empty slot first
            int victim = -1;
            for (int j = 0; j < n_frames; j++) {
                if (frames[j] == -1) {
                    victim = j;
                    break;
                }
            }
 
            //No empty slot: find the least recently used frame
            if (victim == -1) {
                int lru_time = last_used[0];
                victim = 0;
                for (int j = 1; j < n_frames; j++) {
                    if (last_used[j] < lru_time) {
                        lru_time = last_used[j];
                        victim   = j;
                    }
                }
            }
 
            evicted          = frames[victim];
            frames[victim]   = page;
            last_used[victim] = i;
        }
 
        if (trace) {
            printf("%d\t%d\t", i + 1, page);
            print_frames(frames, n_frames);
            if (fault) {
                if (evicted == -1) {
                    printf("  Yes");
                } else {
                    printf("\tYes\t(%d removed)\n", evicted);
                }
            } else {
                printf("\tNo\n");
            }
            printf("\n");
        }
    }
    return faults;
}
 
//OPT

int opt(int *pages, int n, int n_frames, int trace) {
    int frames[MAX_FRAMES];
    int faults = 0;
 
    for (int i = 0; i < n_frames; i++) {
        frames[i] = -1;
    }
 
    if (trace) {
        printf("\n=== OPT Trace (%d frames) ===\n", n_frames);
        printf("Step\tPage\tFrames\t\tFault?\tEvicted\n");
        printf("-----------------------------------------------------------\n");
    }
 
    for (int i = 0; i < n; i++) {
        int page    = pages[i];
        int fault   = 0;
        int evicted = -1;
 
        if (!in_frames(frames, n_frames, page, NULL)) {
            fault = 1;
            faults++;
 
            //Look for an empty slot first 
            int victim = -1;
            for (int j = 0; j < n_frames; j++) {
                if (frames[j] == -1) {
                    victim = j;
                    break;
                }
            }
 
            //No empty slot: evict the page used farthest in the future 
            if (victim == -1) {
                int farthest = -1;
                for (int j = 0; j < n_frames; j++) {
                    int next_use = n;  //n: not used again
                    for (int k = i + 1; k < n; k++) {
                        if (pages[k] == frames[j]) {
                            next_use = k;
                            break;
                        }
                    }
                    if (next_use > farthest) {
                        farthest = next_use;
                        victim   = j;
                    }
                }
            }
 
            evicted        = frames[victim];
            frames[victim] = page;
        }
 
        if (trace) {
            printf("%d \t\t%d", i + 1, page);
            print_frames(frames, n_frames);
            if (fault) {
                if (evicted == -1) {
                    printf("\tYes\n");
                } else {
                    printf("\tYes\t(%d removed)\n", evicted);
                }
            } else {
                printf("\tNo\n");
            }
        }
    }
    return faults;
}

//Summary

void print_summary(int *pages, int n) {
    int frame_counts[] = {3, 5, 10};
    int num_fc = 3;
 
    printf("\n========================================================\n");
    printf("  SUMMARY -- Page Faults\n");
    printf("========================================================\n");
    printf("Frames\tFIFO\tLRU\tOPT\n");
    printf("------------------------------------\n");
 
    for (int f = 0; f < num_fc; f++) {
        int frame_count = frame_counts[f];
        int faults_fifo = fifo(pages, n, frame_count, 0);
        int faults_lru = lru (pages, n, frame_count, 0);
        int faults_opt = opt (pages, n, frame_count, 0);
        printf("%d\t%d\t%d\t%d\n", frame_count, faults_fifo, faults_lru, faults_opt);
 
    printf("========================================================\n");
}
}

//Input
int parse_string(char *buf, int *pages) {
    int n = 0;
    char *tok = strtok(buf, ", \t\n\r");
    while (tok && n < MAX_PAGES) {
        pages[n++] = atoi(tok);
        tok = strtok(NULL, ", \t\n\r");
    }
    return n;
}

//Main
int main(void) {
    int pages[MAX_PAGES];
    int n;
    char buf[512];
 
    printf("=======================================================\n");
    printf("  Page Replacement Algorithm Simulator\n");
    printf("  Algorithms: FIFO | LRU | OPT\n");
    printf("=======================================================\n\n");
 
    printf("Enter page reference string (comma or space separated):\n> ");
    if (!fgets(buf, sizeof(buf), stdin)) {
        fprintf(stderr, "Input error.\n");
        return 1;
    }
 
    n = parse_string(buf, pages);
    if (n == 0) {
        fprintf(stderr, "No pages parsed. Exiting.\n");
        return 1;
    }
 
    printf("\nReference string (%d pages): ", n);
    for (int i = 0; i < n; i++) {
        printf("%d%s", pages[i], i < n - 1 ? " " : "\n");
    }
 
    /* --- Trace section --- */
    int trace_frames;
    printf("\nEnter number of frames for detailed trace (e.g. 3): ");
    if (!fgets(buf, sizeof(buf), stdin) || (trace_frames = atoi(buf)) < 1) {
        trace_frames = 3;
    }
 
    int alg_choice;
    printf("Select algorithm for trace  1=FIFO  2=LRU  3=OPT  0=All: ");
    if (!fgets(buf, sizeof(buf), stdin)) {
        alg_choice = 0;
    } else {
        alg_choice = atoi(buf);
    }
 
    if (alg_choice == 0 || alg_choice == 1) {
        fifo(pages, n, trace_frames, 1);
    }
    if (alg_choice == 0 || alg_choice == 2) {
        lru(pages, n, trace_frames, 1);
    }
    if (alg_choice == 0 || alg_choice == 3) {
        opt(pages, n, trace_frames, 1);
    }
 
    //output summary table
    print_summary(pages, n);
 
    return 0;
}