#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t mutexes[NUM_PHILOSOPHERS];
pthread_cond_t cond_vars[NUM_PHILOSOPHERS];
int forks[NUM_PHILOSOPHERS];

void initialize() {
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&mutexes[i], NULL);
        pthread_cond_init(&cond_vars[i], NULL);
        forks[i] = 1;  // 1 indicates the fork is available
    }
}

void think(int philosopher) {
    printf("Philosopher %d is thinking.\n", philosopher);
    sleep(1 + (rand() % 3));
}

void eat(int philosopher) {
    printf("Philosopher %d is eating.\n", philosopher);
    sleep(1 + (rand() % 3));
}

void pick_up_forks(int philosopher) {
    
}

void put_down_forks(int philosopher) {

}

void* philosopher(void* num) {
    int philosopher = *(int*)num;
    while (1) {
        think(philosopher);
        pick_up_forks(philosopher);
        eat(philosopher);
        put_down_forks(philosopher);
    }
    return NULL;
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosopher_numbers[NUM_PHILOSOPHERS];

    initialize();

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_numbers[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_numbers[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}
