#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_CHAIRS 5  // Number of chairs in the waiting room
#define NUM_CUSTOMERS 10 // Number of customers

pthread_mutex_t mutex;    // Mutex to protect shared resources
pthread_cond_t barber_ready;  // Condition variable to signal barber
pthread_cond_t customer_ready; // Condition variable to signal customer

int available_chairs = NUM_CHAIRS; // Number of available chairs
int waiting_customers = 0; // Count of waiting customers

void* barber(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        while (waiting_customers == 0) {
            printf("Barber: No customers, sleeping.\n");
            pthread_cond_wait(&customer_ready, &mutex);
        }

        // Serving a customer
        waiting_customers--;
        available_chairs++;

        printf("Barber: A customer is being served. Chairs available: %d\n", available_chairs);

        // Signal the customer
        pthread_cond_signal(&barber_ready);
        pthread_mutex_unlock(&mutex);

        // Sleep random time for haircut execution time
        sleep(rand() % 5 + 1);
        printf("Barber: Finished haircut.\n");
    }

    return NULL;
}

void* customer(void* arg) {
    int id = *(int*)arg;

    pthread_mutex_lock(&mutex);
    if (available_chairs > 0) {
        available_chairs--; // Take a chair
        waiting_customers++; // Increment waiting customers
        printf("Customer %d: Waiting. Chairs available: %d\n", id, available_chairs);

        // Signal the barber that a customer is ready
        pthread_cond_signal(&customer_ready);

        // Wait for the barber to serve
        pthread_cond_wait(&barber_ready, &mutex);
        pthread_mutex_unlock(&mutex);
    } else {
        printf("Customer %d: No chairs available, leaving.\n", id);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t barber_thread;
    pthread_t customer_threads[NUM_CUSTOMERS];

    pthread_mutex_init(&mutex, NULL); // Initialize mutex
    pthread_cond_init(&barber_ready, NULL); // Initialize condition variable
    pthread_cond_init(&customer_ready, NULL); // Initialize condition variable

    // Create barber thread
    pthread_create(&barber_thread, NULL, barber, NULL);

    // Create customer threads
    int customer_ids[NUM_CUSTOMERS];
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        customer_ids[i] = i + 1;
        pthread_create(&customer_threads[i], NULL, customer, &customer_ids[i]);
        sleep(rand() % 2); // Random arrival time for customers
    }

    // Join customer threads
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customer_threads[i], NULL);
    }

    // Clean up
    pthread_cancel(barber_thread); // End the barber thread
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&barber_ready);
    pthread_cond_destroy(&customer_ready);

    return 0;
}
