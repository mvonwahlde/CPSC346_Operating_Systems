/*
* File:           test.c
* Author:         Matthew VonWahlde
* Date:           9/8/23
* Description:    The c code file for the testing of the mergesort implementation
*/

#include "test.h"


// Tests to see if mergesort can handle an empty list
void testEmptyList(){
    // Print out testing header
    printf("\n--------------------\n");
    printf("EMPTY LIST TEST\n");
    printf("--------------------\n");

    // Create the original list and print
    ListNode *head = NULL;
    printf("\nOriginal List: ");
    printList(head);
    
    // Sort the list and print out the sorted list
    ListNode *ret = mergesort(head, 0);
    printf("\nSorted List: ");
    printList(ret);

    // Print the results section
    printf("\nRESULTS: ");

    // If both the original and sorted list are NULL, the test has passed
    if(head == NULL && ret == NULL){
        printf("PASSED\n");
    } else {
        // If not, the test failed
        printf("FAILED\n");
    }

    // Print line to show end of test
    printf("--------------------\n\n");

    return;
}


// Tests to see if mergesort can handle a one item list
void testOneItemList(){
    // Print out test header
    printf("\n--------------------\n");
    printf("ONE ITEM LIST TEST\n");
    printf("--------------------\n");

    // Size of the list
    int size = 1;

    // Generate a random array of size 1 to turn into a list
    int arr[size];
    generateRandArr(arr, size);

    // Create the original list and print
    ListNode *head = createList(arr, size);
    printf("\nOriginal List: ");
    printList(head);
    
    // Sort the list and print the sorted list
    ListNode *ret = mergesort(head, 1);
    printf("\nSorted List: ");
    printList(ret);

    // Print the results section
    printf("\nRESULTS: ");

    // If the two lists have a single node that is the same value, the test passed
    if(head->data == ret->data && (head->next == NULL && ret->next == NULL)){
        printf("PASSED\n");
    } else {
        // If not, the test failed
        printf("FAILED\n");
    }

    // Print line to show end of test
    printf("--------------------\n\n");

    deleteList(ret);

    return;
}


// Tests a varying size list to see if mergesort is sorting the list correctly
void testList(int size){
    // Print out test header
    printf("\n--------------------\n");
    printf("%d ELEMENT LIST TEST\n", size);
    printf("--------------------\n");

    // Generate a random array to turn into a list
    int arr[size];
    generateRandArr(arr, size);

    // Create the original list and print
    ListNode *head = createList(arr, size);
    printf("\nOriginal List: ");
    printList(head);
    
    // Sort the list and print the sorted list
    ListNode *ret = mergesort(head, size);
    printf("\nSorted List: ");
    printList(ret);

    // Print the results section
    printf("\nRESULTS: ");

    // If the list is sorted, the test passed
    if(isSorted(ret)){
        printf("PASSED\n");
    } else {
        // If not, the test failed
        printf("FAILED\n");
    }

    // Print line to show end of test
    printf("--------------------\n\n");

    // Delete the list before returning
    deleteList(ret);

    return;
}


// Creates a linked list from an array
ListNode* createList(int *arr, int len){
    // Pointer to the head of the returned linked list and a pointer to traverse
    ListNode *head;
    ListNode *tmp;

    // If the len is 0 (or less) return NULL
    if(len < 1)
        return NULL;

    // Create a node and have head point to it
    head = (ListNode *)malloc(sizeof(ListNode));
    head->data = arr[0];

    // Set tmp equal to head to traverse
    tmp = head;

    // For the remaining array values
    for(int i = 1; i < len; i++){
        // Set the next value as a new node, traverse to that node, and then set the data value
        tmp->next = (ListNode *)malloc(sizeof(ListNode));
        tmp = tmp->next;
        tmp->data = arr[i];
    }

    // Set the end of the list to NULL
    tmp->next = NULL;

    return head;
}


// Deletes a linked list recursively
void deleteList(ListNode* list){
    // Stop case: if the input is NULL, we have reached the end of the list
    if(list == NULL){ 
        return;
    }

    // Delete the next item in the list
    deleteList(list->next);

    // After the stop case has been reached, each function call will delete the node the point to
    // This deletes the list from back to front
    free(list);
}


// Prints a list to the terminal
void printList(ListNode *list){
    // If the list is NULL, give some indicator
    if (list == NULL){
        printf("NULL");
    }

    // While the current node is not NULL
    while(list != NULL){
        // Print the data at that node and then go to the next node
        printf("%i ", list->data);
        list = list->next;
    }

    // Print a new line character to end the line
    printf("\n");

    return;
}


// Fill an array with random numbers between 0 and 999 inclusive
void generateRandArr(int *arr, int size){
    // For each element in the array, generate a random number between 0 and 999 inclusive
    // Note: merge sort still works with larger numbers, this is for readability purposes
    for(int i = 0; i < size; i++){
        arr[i] = rand() % 1000;
    }
}


// Check to see if a list is sorted in non-descending order
int isSorted(ListNode *head){
    // If the list contains one or no elements, it is already sorted
    if(head == NULL || head->next == NULL){
        return 1;
    }

    // While the next node is not NULL
    while(head->next != NULL){
        // Check if the current value is greater than the next value
        if(head->data > head->next->data)
            return 0; // List is not sorted

        // Traverse to the next node
        head = head->next;
    }

    // If the while loop completed without returning, then the list was sorted
    return 1;
}
