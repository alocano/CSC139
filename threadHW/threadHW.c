/****************************************************/
/*Althaea Locano CSC 139-01 HW, April 9, 2026       */
/*Assignment: Work on thread utilization while      */
/*simulating a multi-threaded web server            */
/*Implement a mutex (a), condition variable (b), and*/
/*semaphore (c) to protect shared resources         */
/****************************************************/

// initalization (libraries and global variable config)
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 10
#define NUM_PRODUCERS 4
#define NUM_CONSUMERS 4
#define REQUESTS 5

int global_request_id = 0; //global request counter

// shared buffer/queue
int buffer[BUFFER_SIZE];
int head = 0;
int tail = 0;
int count = 0;

// synchronization primitives
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_not_empty = PTHREAD_COND_INITIALIZER;

sem_t sem_empty;
sem_t sem_full;

// producer thread function
void* producer(void* arg){
    int producer_id = *((int*)arg);
    
    for(int i = 0; i < REQUESTS; i++){
        //c. semaphore wait for empty slot
        sem_wait(&sem_empty);
        // printf("[SEM] PRODUCER %d: acquired empty slot\n", producer_id);
        
        //a. lock mutex
        pthread_mutex_lock(&mutex);
        // printf("[MUTEX] PRODUCER %d: locked\n", producer_id);

        //b. conditional
        while(count == BUFFER_SIZE){
            pthread_cond_wait(&cond_not_full, &mutex);
           // printf("[COND] PRODUCER %d: waiting for not full\n", producer_id);
        }

        // printf("[COND] PRODUCER %d: got not full signal\n", producer_id);
        fflush(stdout);

        global_request_id++;
        int request_id = global_request_id; //assign request id

        //add request to buffer
        buffer[tail] = request_id;
        tail = (tail + 1) % BUFFER_SIZE;
        count++;

        printf("[PRODUCER %d] Sent request %d | Buffer Count: %d\n", producer_id, request_id, count); 
        fflush(stdout);

        pthread_mutex_unlock(&mutex); //unlock mutex
        pthread_cond_signal(&cond_not_empty); //signal consumer
        sem_post(&sem_full); //signal that a new request is added

        usleep(100000); //delay for 100ms between requests 
    }
    return NULL;
}

// consumer thread function
void* consumer(void* arg){
    int consumer_id = *((int*)arg);

    int to_consume = (NUM_PRODUCERS * REQUESTS) / NUM_CONSUMERS; 

    for(int i = 0; i < to_consume; i++){
        //c. semaphore wait for full slot
        sem_wait(&sem_full);
        // printf("[SEM] CONSUMER %d: acquired full slot\n", consumer_id);
        //a. lock mutex
        pthread_mutex_lock(&mutex);
        // printf("[MUTEX] CONSUMER %d: locked\n", consumer_id);

        //b. conditional
        while(count == 0){
            pthread_cond_wait(&cond_not_empty, &mutex);
            // printf("[COND] CONSUMER %d: waiting for not empty\n", consumer_id);
        }
        // printf("[COND] CONSUMER %d: got not empty signal\n", consumer_id);
        fflush(stdout);

        int request_id = buffer[head]; //consume request
        head = (head + 1) % BUFFER_SIZE;
        count--;

        printf("[CONSUMER %d] Processed request %d | Buffer Count: %d\n", consumer_id, request_id, count);
        fflush(stdout);

        pthread_cond_signal(&cond_not_full); 
        pthread_mutex_unlock(&mutex); 
        sem_post(&sem_empty);

        usleep(100000); //delay for 100ms between processing
    }
    return NULL;
}

// main function
int main(void){
printf("PRODUCERS: %d | REQUESTS EACH %d | TOTAL REQUESTS: %d\n", NUM_PRODUCERS, REQUESTS, NUM_PRODUCERS * REQUESTS);
    printf("CONSUMERS: %d\n", NUM_CONSUMERS);
    printf("BUFFER SIZE: %d\n", BUFFER_SIZE);
    printf("==============================================================\n");
    fflush(stdout);

    //error handling
    if(sem_init(&sem_empty, 0, BUFFER_SIZE) != 0){
        perror("Failed to initialize sem_empty");
        exit(1);
    }
    if(sem_init(&sem_full, 0, 0) != 0){
        perror("Failed to initialize sem_full");
        exit(1);
    }

    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    for(int i = 0; i<NUM_CONSUMERS; i++){
        int* id = malloc(sizeof(int));
        *id = i + 1;
        if(id == NULL){
            perror("Failed to allocate memory for consumer ID");
            exit(1);
        }
        if(pthread_create(&consumers[i], NULL, consumer, id) != 0){
            perror("Failed to create consumer thread");
            free(id);
            exit(1);
        }
    }

    for(int i = 0; i < NUM_PRODUCERS; i++){
        int *id = malloc(sizeof(int));
        if(id == NULL){
            perror("Failed to allocate memory for producer ID");
            exit(1);
        }
        *id = i + 1;

        if(pthread_create(&producers[i], NULL, producer, id) != 0){
            perror("Failed to create producer thread");
            free(id);
            exit(1);
        }
    }

    for(int i = 0; i < NUM_PRODUCERS; i++){
        if(pthread_join(producers[i], NULL) != 0){
            perror("Failed to join producer thread");
            exit(1);
        }
    }
    for(int i = 0; i < NUM_CONSUMERS; i++){
        if(pthread_join(consumers[i], NULL) != 0){
            perror("Failed to join consumer thread");
            exit(1);
        }
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_not_full);
    pthread_cond_destroy(&cond_not_empty);

    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);

    printf("\n=== SIMULATION COMPLETE === \n");
    printf("All %d requests produced and processed. Exiting...\n", NUM_PRODUCERS * REQUESTS);

    exit(0);
}
