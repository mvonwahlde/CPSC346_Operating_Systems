/*
* File:           test.h
* Author:         Matthew VonWahlde
* Date:           9/8/23
* Description:    The header file for the testing of the mergesort implementation
*/

#ifndef TEST_H
#define TEST_H


#include "stdio.h"
#include <stdlib.h>
#include <time.h>
#include "merge.h"

// MACRO used when testing functions
//#define ARR_SIZE(arr)  (int)(sizeof(arr)/sizeof(int))

/*
* Function:     testEmptyList
* Parameters:   None
* Return:       None
* Description:  Tests merge sort algorithm for an empty list, prints results to terminal
*/
void testEmptyList();

/*
* Function:     testOneItemList
* Parameters:   None
* Return:       None
* Description:  Tests merge sort algorithm for a list with 1 item, prints results to terminal
*/
void testOneItemList();

/*
* Function:     testList
* Parameters:   An integer that represents the desired size of the list to test
* Return:       None
* Description:  Tests merge sort algorithm for a list with multiple items, prints results to terminal
*/
void testList(int size);

/*
* Function:     createList
* Parameters:   An pointer to an integer array (the desired values for the list nodes)
*               An integer specifying the size of the array and size of the returning list
* Return:       Returns a pointer to the start of the linked list created using the integer array
* Description:  Takes an integer array and turns that into a linked list with the same values and in the same order
*/
ListNode* createList(int *arr, int len);

/*
* Function:     deleteList
* Parameters:   A pointer to a linked list
* Return:       None
* Description:  Deletes the nodes in a linked list
*/
void deleteList(ListNode* list);

/*
* Function:     printList
* Parameters:   A pointer to the start of a linked list
* Return:       None
* Description:  Prints the contents of a linked list to the terminal
*/
void printList(ListNode *list);

/*
* Function:     generateRandArr
* Parameters:   An pointer to an integer array to fill with random values (0-999)
*               An integer specifying the size of the array
* Return:       None
* Description:  Fills an array with random integer values from 0 to 999
*/
void generateRandArr(int *arr, int size);

/*
* Function:     isSorted
* Parameters:   A pointer to the start of a linked list
* Return:       An integer: returns 1 is the list is sorted, returns 0 if the list is not sorted
* Description:  Checks if the linked list is sorted. 
*               If it is sorted, then 1 is returned; if it is not sorted, then 0 is returned
*/
int isSorted(ListNode *head);


#endif