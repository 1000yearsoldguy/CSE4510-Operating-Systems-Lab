#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t mutexes[NUM_PHILOSOPHERS];
pthread_cond_t cond_vars[NUM_PHILOSOPHERS];
int forks[NUM_PHILOSOPHERS];

void initialize()
{
  for (int i = 0; i < NUM_PHILOSOPHERS; i++)
  {
    pthread_mutex_init(&mutexes[i], NULL);
    pthread_cond_init(&cond_vars[i], NULL);
    forks[i] = 1; // 1 indicates the fork is available
  }
}

void think(int philosopher)
{
  printf("Philosopher %d is thinking.\n", philosopher);
  sleep(1 + (rand() % 3));
}

void eat(int philosopher)
{
  printf("Philosopher %d is eating.\n", philosopher);
  sleep(1 + (rand() % 3));
}

void pick_up_forks(int philosopher){
  int right = (philosopher + 1) % NUM_PHILOSOPHERS; // Right fork index
  int left = philosopher;                           // Left fork index

  // Lock the mutex for the philosopher
  pthread_mutex_lock(&mutexes[philosopher]);

  // Wait until both forks are available
  while (forks[right] == 0 || forks[left] == 0)
  {
    pthread_cond_wait(&cond_vars[philosopher], &mutexes[philosopher]);
  }

  // Mark both forks as taken
  forks[right] = 0;
  forks[left] = 0;

  printf("Philosopher %d Picked Up %d & %d number Forks \n", philosopher, right, left);

  // Unlock the mutex
  pthread_mutex_unlock(&mutexes[philosopher]);
}

void put_down_forks(int philosopher)
{
  int right = (philosopher + 1) % NUM_PHILOSOPHERS; // Right fork index
  int left = philosopher;                           // Left fork index

  // Lock the mutex for the philosopher
  pthread_mutex_lock(&mutexes[philosopher]);

  // Mark both forks as available
  forks[right] = 1;
  forks[left] = 1;

  printf("Philosopher %d Put Down %d & %d number Forks \n", philosopher, right, left);

  // Signal the adjacent philosophers
  pthread_cond_signal(&cond_vars[(philosopher + 1) % NUM_PHILOSOPHERS]);                    // Notify right neighbor
  pthread_cond_signal(&cond_vars[(philosopher + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS]); // Notify left neighbor

  // Unlock the mutex
  pthread_mutex_unlock(&mutexes[philosopher]);
}

void *philosopher(void *num)
{
  int philosopher = *(int *)num;
  while (1)
  {
    think(philosopher);
    pick_up_forks(philosopher);
    eat(philosopher);
    put_down_forks(philosopher);
  }
  return NULL;
}

int main()
{
  pthread_t philosophers[NUM_PHILOSOPHERS];
  int philosopher_numbers[NUM_PHILOSOPHERS];

  initialize();

  for (int i = 0; i < NUM_PHILOSOPHERS; i++)
  {
    philosopher_numbers[i] = i;
    pthread_create(&philosophers[i], NULL, philosopher, &philosopher_numbers[i]);
  }

  for (int i = 0; i < NUM_PHILOSOPHERS; i++)
  {
    pthread_join(philosophers[i], NULL);
  }

  return 0;
}
