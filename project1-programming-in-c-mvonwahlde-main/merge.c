/*
* File:           merge.c
* Author:         Matthew VonWahlde
* Date:           9/8/23
* Description:    The c code file for the mergesort functions
*/

#include "merge.h"


// Mergesort algorithm: sorts inputted list
ListNode* mergesort(ListNode *head, int size){
    // If the list contains 0 or 1 items, it is already sorted
    if(head == NULL){
        return head;
    } else if(head->next == NULL){
        return head;
    }

    // Sizes of the two lists split from the inputted list
    int size1 = size/2;
    int size2 = size - size1;

    // Containers to hold the two lists split from the inputted list
    ListNode *list1 = NULL;
    ListNode *list2 = NULL;

    // Split the list into two seperate lists
    splitList(head, &list1, size1, &list2);

    // Rescursively call mergesort to keep splitting the list until each list is only 0 or 1 elements
    // Then, merge those sorted lists into one and return
    return merge(mergesort(list1, size1), mergesort(list2, size2));
}


// Splits a list into two lists base on the desired size of the first list
void splitList(ListNode *head, ListNode **list1, int size1, ListNode **list2){
    // Set the start of list 1 to the start of the inputted list
    *list1 = head;

    // ListNode pointer to traverse the first list (also the main list)
    ListNode *tmp1 = *list1;
    
    // Add size1 nodes from the original list to list1
    for(int i = 1; i < size1; i++){
        // If tmp1 is NULL, return as there is an error
        if(tmp1 == NULL)
            return;
        
        // Traverse to the next point
        tmp1 = tmp1->next;
    }

    // If tmp1 is NUL, return as there is an error
    if(tmp1 == NULL)
        return;

    // Set the start of list2 to the value after the end of list 1
    *list2 = tmp1->next;

    // Set the end of the first list to NULL
    tmp1->next = NULL;

    return;
}


// Merges two lists together in non-descending order
ListNode* merge(ListNode *list1, ListNode *list2){
    // Create a new list node pointer to return
    ListNode* newHead;

    // Add the next value to the return list
    if(list1 == NULL){
        // If the first list has reached the end, return the second list
        return list2;
    } else if(list2 == NULL){
        // If the second list has reached the end, return the first list
        return list1;
    } else {
        // If neither list has reached the end

        // Check which list to add the new node from
        if(list1->data <= list2->data){ 
            // If the node from the first list is less than or equal to the node of the second list,
            // add the node from the first list and then recursively call merge to set the next node 
            newHead = list1;
            newHead->next = merge(list1->next, list2);
        } else {
            // If the node from the second list is less than the node of the first list,
            // add the node from the second list and then recursively call merge to set the next node 
            newHead = list2;
            newHead->next = merge(list1, list2->next);
        }
    }

    return newHead;
}
