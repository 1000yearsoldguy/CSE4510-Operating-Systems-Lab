#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

const int K = 5; // Bus capacity
int waiting_passengers = 0; // Number of passengers waiting at the bus stop
int boarded_passengers = 0; // Number of passengers boarded on the bus

pthread_mutex_t mutex;
pthread_cond_t bus_arrived;
pthread_cond_t passengers_boarded;

void* bus(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        // Bus arrives at the stop
        printf("Bus: Arrived at the bus stop. Waiting passengers: %d\n", waiting_passengers);
        while (waiting_passengers > 0 && boarded_passengers < K) {
            pthread_cond_broadcast(&bus_arrived); // Signal passengers to board
            pthread_cond_wait(&passengers_boarded, &mutex); // Wait for passengers to board
        }

        // Bus departs
        printf("Bus: Departing with %d passengers.\n", boarded_passengers);
        boarded_passengers = 0; // Reset boarded passengers

        pthread_mutex_unlock(&mutex);

        // Simulate bus travel time
        sleep(rand()%6+2);
    }
    return NULL;
}

void* passenger(void* arg) {
    // Extract the passenger ID from the argument
    int id = *(int*)arg;

    // Lock the mutex to update shared variables
    pthread_mutex_lock(&mutex);

    // Print a message indicating the passenger is waiting at the bus stop
    printf("Passenger %d: Waiting at the bus stop.\n", id);
    
    // Increment the count of waiting passengers
    waiting_passengers++;

    // Wait until the bus arrives and there is space for the passenger to board
    pthread_cond_wait(&bus_arrived, &mutex);

    // Decrement the count of waiting passengers
    waiting_passengers--;
    
    // Increment the count of boarded passengers
    boarded_passengers++;
    
    // Print a message indicating the passenger has boarded the bus
    printf("Passenger %d: Boarded the bus.\n", id);

    // If there are no more waiting passengers or the bus is full, signal that boarding is complete
    if (waiting_passengers == 0 || boarded_passengers == K) 
        pthread_cond_signal(&passengers_boarded);

    // Unlock the mutex to allow other threads to access shared variables
    pthread_mutex_unlock(&mutex);

    // Return from the function
    return NULL;
}


int main() {
    pthread_t bus_thread;
    pthread_t passenger_threads[10];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&bus_arrived, NULL);
    pthread_cond_init(&passengers_boarded, NULL);

    // Create bus thread
    pthread_create(&bus_thread, NULL, bus, NULL);

    // Create passenger threads
    int passenger_ids[10];
    for (int i = 0; i < 10; i++) {
        passenger_ids[i] = i + 1;
        pthread_create(&passenger_threads[i], NULL, passenger, &passenger_ids[i]);
        sleep(1); // Simulate random passenger arrival times
    }

    // Join passenger threads
    for (int i = 0; i < 10; i++) {
        pthread_join(passenger_threads[i], NULL);
    }

    // Clean up
    pthread_cancel(bus_thread);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&bus_arrived);
    pthread_cond_destroy(&passengers_boarded);

    return 0;
}
