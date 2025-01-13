#include <stdio.h>      // Standard input/output library for printf function
#include <pthread.h>    // POSIX thread library for threading functions
#include <stdlib.h>     // Standard library for general functions like malloc, free, etc.
#include <unistd.h>     // POSIX operating system API for functions like sleep

#define NumberOfThreads 26  // Define a constant for the number of threads, which is 26

pthread_mutex_t mutex;  // Declare a mutex to protect shared resources
pthread_cond_t conditional_variables[NumberOfThreads];  // Declare an array of condition variables, one for each thread
int current_letter = 0;  // Variable to keep track of the current letter to be printed

// Thread function to print a letter
void *print_letter(void *argument) {
    int threadID = *(int *)argument;  // Extract the thread ID from the argument
    char letter = 'A' + threadID;  // Calculate the letter to be printed based on the thread ID

    pthread_mutex_lock(&mutex);  // Lock the mutex to protect shared resources

    while (threadID != current_letter) {  // Wait until it is this thread's turn to print
        pthread_cond_wait(&conditional_variables[threadID], &mutex);  // Wait on the condition variable
    }

    printf("Thread - %d: %c\n", threadID + 1, letter);  // Print the thread ID and the letter
    current_letter++;  // Increment the current letter to the next one
    pthread_cond_signal(&conditional_variables[current_letter]);  // Signal the next thread
    pthread_mutex_unlock(&mutex);  // Unlock the mutex

    return NULL;  // Return NULL to indicate the end of the thread function
}

int main() {
    pthread_t threads[NumberOfThreads];  // Declare an array to hold thread identifiers
    int indices[NumberOfThreads];  // Declare an array to hold thread indices

    pthread_mutex_init(&mutex, NULL);  // Initialize the mutex

    for (int i = 0; i < NumberOfThreads; i++) {
        indices[i] = i;  // Set the index for each thread
        pthread_create(&threads[i], NULL, print_letter, &indices[i]);  // Create a thread with the print_letter function and pass the index as an argument
        pthread_cond_init(&conditional_variables[i], NULL);  // Initialize each condition variable
    }

    pthread_mutex_lock(&mutex);  // Lock the mutex to start the process
    pthread_cond_signal(&conditional_variables[0]);  // Signal the first thread to start
    pthread_mutex_unlock(&mutex);  // Unlock the mutex

    for (int i = 0; i < NumberOfThreads; i++) {
        pthread_join(threads[i], NULL);  // Wait for each thread to finish
    }

    for (int i = 0; i < NumberOfThreads; i++) {
        pthread_cond_destroy(&conditional_variables[i]);  // Destroy each condition variable
    }
    pthread_mutex_destroy(&mutex);  // Destroy the mutex

    return 0;  // Return 0 to indicate successful execution of the program
}