/**
 * Author:      Matthew VonWahlde
 * Date:        10/30/23
 * Class:       CPSC 346 02
 * Description: Project 6 - Multithreaded sorting
 * 				Divide the list in half, and sort each of those sections using a thread.
 * 				Use another thread to merge the two lists together.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE				10   /* # of elements in the list   */
#define NUMBER_OF_THREADS	3    /* # of threads in the program */

void *sorter(void *params);    	 /* thread that performs basic sorting algorithm */
void *merger(void *params);	     /* thread that performs merging of results */

int list[SIZE] = {7,12,19,3,18,4,2,6,15,8};
int result[SIZE];

typedef struct {
	int from_index;
	int to_index;
} parameters;


// Main program
int main (int argc, const char * argv[]) {
    
	// Thread list
	pthread_t workers[NUMBER_OF_THREADS];

	// Print the original array
	printf("Original Array: ");
	for(int i = 0; i < SIZE; i++){
		printf("%d ", list[i]);
	}
	printf("\n");

	// Prepare parameters for the first sorting thread and create it
	parameters *params1 = (parameters *)malloc(sizeof(parameters));
	params1->from_index = 0;
	params1->to_index = (SIZE-1)/2;
	pthread_create(&workers[0], 0, sorter, (void *)params1);

	// Prepare parameters for the second sorting thread and create it
	parameters *params2 = (parameters *)malloc(sizeof(parameters));
	params2->from_index = ((SIZE-1)/2) + 1;
	params2->to_index = SIZE - 1;
	pthread_create(&workers[1], 0, sorter, (void *)params2);
	
	// Wait for the two threads to finish 
	for(int i = 0; i < 2; i++){
		pthread_join(workers[i], 0);
	}
	
	// Prepare parameters for the merging thread and create it
	parameters *params3 = (parameters *)malloc(sizeof(parameters));
	params3->from_index = 0;
	params3->to_index = ((SIZE-1)/2) + 1;
	pthread_create(&workers[2], 0, merger, (void *)params3);
	
	// Wait for the merging thread to finish
	pthread_join(workers[2], 0);

	// Print the sorted array
	printf("Sorted Array: ");
	for(int i = 0; i < SIZE; i++){
		printf("%d ", result[i]);
	}
	printf("\n");
	
    return 0;
}


/**
 * Sorting thread.
 *
 * This thread can essentially use any algorithm for sorting
 */
void *sorter(void *params) {
	// Create a copy of the parameters as the proper type
	parameters* p = (parameters *)params;

	// Easier to read
	int from = p->from_index;
	int to   = p->to_index;

	// Sorting algorithm here (Selection Sort)
	for(int i = from; i < to; i++){
		int smallest = list[i];
		int idx = i;

		// If the current element is smaller than element i, set smallest
		for(int j = i+1; j <= to; j++){
			if(list[j] < smallest){
				smallest = list[j];
				idx = j;
			}
		}	

		// If there was a smaller element later in the sublist, swap it with the ith element
		if(i != idx){
			int tmp = list[i];
			list[i] = smallest;
			list[idx] = tmp;
		}
	}

	pthread_exit(0);
}


/**
 * Merge thread
 *
 * Uses simple merge sort for merging two sublists
 */
void *merger(void *params) {
	// Create a copy of the parameters as the proper type
	parameters* p = (parameters *)params;
	
	// Indexes for the first and second lists
	int l1 = p->from_index;
	int l2 = p->to_index;

	// Merge two sorted sublist to the arry result
	for(int i = 0; i < SIZE; i++){
		if(l1 == p->to_index) {
			// If l1 is exhausted, use l2
			result[i] = list[l2++];
		} else if(l2 == SIZE) {
			// If l2 is exhausted, use l1
			result[i] = list[l1++];
		} else {
			// If both list still has elements remaining, find the smallest
			result[i] = (list[l1] <= list[l2]) ? list[l1++] : list[l2++];
		}
	}

	pthread_exit(0);
}
