/*
File:    buffer.c
Author:  Matthew VonWahlde
Date:    November 10, 2023
Class:   CPSC 346 02
Project: Project 7 - Producer Consumer
Description:
	The user inputs three numbers, the sleepTime which is the duration of the program, the number of producer threads, and the number of consumer threads.
	Each producer thread will wait a random amount of time and then place an item in the buffer. Each consumer thread will wait a random amount of time and then 
	take an item from the buffer. Semaphores and mutexes are implemented for thread synchronization
*/

#include "buffer.h"

// Shared buffer among threads
buffer_item buffer[BUFFER_SIZE];

// Semaphores and mutexes
pthread_mutex_t mutex; // Buffer protection
sem_t empty;           // Number of empty spots in the buffer
sem_t full;            // Number of occupied spots in the buffer

// Buffer indexes for placing and removing items
int insertPointer = 0, removePointer = 0;

// Threads' function prototypes
void *producer(void *param);
void *consumer(void *param);

/*
Function: insert_item()
Inputs:   buffer_item item - item to place in the buffer
Output:   int - error code (0 OK, -1 error)
Description:
	Inserts the inputted item into the buffer, waiting for and releasing necessary semaphores and mutex
*/
int insert_item(buffer_item item){
    // Acquire the empty semaphore (make sure there is open space in the buffer)
	if(sem_wait(&empty))
		return -1;

	// Acquire mutex lock to protect the buffer
	if(pthread_mutex_lock(&mutex))
		return -1;

	// Place item in the buffer
	buffer[insertPointer] = item;

	// Update the insert buffer index
	if(++insertPointer >= BUFFER_SIZE)
		insertPointer = 0;

	// Print out the message here to prevent thread switching before the number is printed
	printf("producer produced %d\n", item);

	// Release the mutex lock and release the full semaphore (one more spot is full)
	if(pthread_mutex_unlock(&mutex))
		return -1;
	
	if(sem_post(&full))
		return -1;

	return 0;
}


/*
Function: remove_item()
Inputs:   buffer_item *item - container to store the item
Output:   int - error code (0 OK, -1 error)
Description:
	Removes item from the buffer and stores in the input pointer, waiting for and releasing necessary semaphores and mutex
*/
int remove_item(buffer_item *item){
	// Acquire the full semaphore (make sure there is something in the buffer)
	if(sem_wait(&full))
		return -1;

	// Acquire the mutex lock to protect the buffer
	if(pthread_mutex_lock(&mutex))
		return -1;

	// Get item from the buffer
	*item = buffer[removePointer];

	// Update the remove index for the buffer
	if(++removePointer >= BUFFER_SIZE)
		removePointer = 0;

	// Print out the message here to prevent thread switching before the number is printed
	printf("consumer consumed %d\n", *item);

	// Release mutex lock and empty semaphore (add another empty spot)
	if(pthread_mutex_unlock(&mutex))
		return -1;
	
	if(sem_post(&empty))
		return -1;

	return 0;
}


// Main - Entry point
int main(int argc, char *argv[]){
	int sleepTime, producerThreads, consumerThreads;
	int i, j;

	srand(time(NULL));

	// If the argument count is incorrect, print message and exit program 
	if(argc != 4)
	{
		fprintf(stderr, "Useage: <sleep time> <producer threads> <consumer threads>\n");
		return -1;
	}

	// Convert the arguments from strings to integers
	sleepTime = atoi(argv[1]);
	producerThreads = atoi(argv[2]);
	consumerThreads = atoi(argv[3]);

	// Initialize mutexes and threads 
	int rc = pthread_mutex_init(&mutex, NULL);
	assert(rc == 0);

	rc = sem_init(&empty, 0, BUFFER_SIZE);
	assert(rc == 0);

	rc = sem_init(&full, 0, 0);
	assert(rc == 0);

	// Create the producer threads
	pthread_t producers[producerThreads];
	for(i = 0; i < producerThreads; i++){
		rc = pthread_create(&producers[i], 0, producer, NULL);
		assert(rc == 0);
	}
	
	// Create the consumer threads
	pthread_t consumers[consumerThreads];
	for(j = 0; j < consumerThreads; j++){
		rc = pthread_create(&consumers[j], 0, consumer, NULL);
		assert(rc == 0);
	}

	// Sleep for user-specified time
	sleep(sleepTime);
	return 0;
}


/*
Function: producer()
Inputs:   void *param - input parameter (none in this case)
Output:   None
Description:
	Producer thread. Waits for random amount of time, then inserts an item into the buffer
*/
void *producer(void *param){
	buffer_item item; 

	// Main loop
	while(TRUE){
		// Wait for a random amount of time (can be limited by defining PRODUCER_MAX_SLEEP)
		#ifdef PRODUCER_MAX_SLEEP_TIME
			// 0 <-> PRODUCER_MAX_SLEEP
			sleep(rand() % (PRODUCER_MAX_SLEEP_TIME+1));
		#else
			// 0 <-> MAX_INT
			sleep(rand());
		#endif

		// Generate a random item and place in the buffer
		item = rand();
		if(insert_item(item))
			fprintf(stderr, "report error condition, Producer");
		// else (printing from inside insert_item())
			// printf("producer produced %d\n", item);
	}
}


/*
Function: consumer()
Inputs:   void *param - input parameter (none in this case)
Output:   None
Description:
	Consumer thread. Waits for random amount of time, then removes an item from the buffer
*/
void *consumer(void *param)
{
	buffer_item item;

	// Main loop
	while(TRUE){
		// Wait for a random amount of time (can be limited by defining CONSUMER_MAX_SLEEP)
		#ifdef CONSUMER_MAX_SLEEP_TIME
			// 0 <-> CONSUMER_MAX_SLEEP
			sleep(rand() % (CONSUMER_MAX_SLEEP_TIME+1));
		#else
			// 0 <-> MAX_INT
			sleep(rand());
		#endif

		// Remove an item from the buffer
		if(remove_item(&item))
			fprintf(stderr, "report error condition, Consumer");
		// else (printing from inside remove_item())
			// printf("consumer consumed %d\n", item);
	}
}
