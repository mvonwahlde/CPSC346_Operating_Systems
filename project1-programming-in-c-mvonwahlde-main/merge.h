/*
* File:           merge.h
* Author:         Matthew VonWahlde
* Date:           9/8/23
* Description:    The header file for the mergesort functions
*/

#ifndef MERGE_H
#define MERGE_H


#include "stdio.h"
#include <stdlib.h>


// ListNode struct. Has an integer and a pointer to the next node
typedef struct ListNode{
    int data;
    struct ListNode *next;
} ListNode;

/*
* Function:     mergesort
* Parameters:   A pointer to the start of a linked list
*               An integer that represents the size of the linked list
* Return:       A pointer to the sorted list
* Description:  Implements a recurive mergesort algorithm to sort the inputted linked list.
*               The resulting linked list will be ordered in non-descending order
*/
ListNode* mergesort(ListNode *head, int size);

/*
* Function:     splitList
* Parameters:   A pointer to the start of a linked list
*               A double pointer to the head of the first list (list will be created here)
*               An integer that represents the desired size of the first list
*               A double pointer to the head of the second list (list will be created here)
* Return:       None (lists created in the two double pointers passed in)
* Description:  Helper function for merge sort. Splits a list into two seperate null terminated.
*               The parameter size1 determines the size of the first list, and the second list is 
*               created with the remaining nodes of the original list
*/
void splitList(ListNode *head, ListNode **list1, int size1, ListNode **list2);

/*
* Function:     merge
* Parameters:   A pointer to the start of the first linked list (in non-decreasing order)
*               A pointer to the start of the second linked list (in non-decreasing order)
* Return:       A pointer to the merged list
* Description:  Merges the two inputted non-decreasing lists to create a single list that is in non-decreasing order
*/
ListNode* merge(ListNode *list1, ListNode *list2);


#endif