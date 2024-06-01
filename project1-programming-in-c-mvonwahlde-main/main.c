/*
* File:           main.c
* Author:         Matthew VonWahlde
* Date:           9/8/23
* Description:    The main file for the mergesort project (project 1)
*/


#include "merge.h"
#include "test.h"


// Main function, calls testing functions
int main(){
    // Set the randomizer seed to get better random values
    srand(time(NULL));
    
    testEmptyList();   // Test an empty list
    testOneItemList(); // Test a list with one item
    testList(5);       // Test a list with 5 items
    testList(20);      // Test a list with 20 items
    testList(1000);    // Test a list with 1000 items

    return 0;
}
