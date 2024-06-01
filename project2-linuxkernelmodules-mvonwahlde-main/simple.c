/*
File Name:      simple.c
Modified By:    Matthew VonWahlde
Last Modified:  September 24, 2023
Decription:     A simple program in c to create a linked list in the kernel on entry, and delete the linked list on exit
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>


// Struct definition for person (linked list node)
typedef struct person{
       int month;
       int day;
       int year;
       struct person *next;
} person;

// Function Prototypes
void createList(void);
void printList(void);
void deleteList(person *cur);

// Global pointer to the head of the linked list
person *head;


/* This function is called when the module is loaded. */
int simple_init(void) {
       // Print loading module message
       printk(KERN_INFO "Loading Module\n");

       // Create the linked list
       createList();

       // Print the linked list
       printList();

       return 0;
}


/* This function is called when the module is removed. */
void simple_exit(void) {
       // Delete the linked list
       deleteList(head);
       head = NULL;

       // Print out the deleted list
       printList();

       // Print removing module message
	printk(KERN_INFO "Removing Module\n");
}


/*
Function:    createList()
Inputs:      None
Outputs:     None
Description: Creates 5 nodes for the linked list and points the global variable head to the beginning
*/
void createList(){
       // Traversing pointer
       person *tmp;
       
       // Create the first node that head points to
       head = (person*)kmalloc(sizeof(person), GFP_KERNEL);
       if(head == NULL){return;} // Error Checking

       // Assign values to the first node
       head->month = 12;
       head->day = 10;
       head->year = 2001;

       // Create the second node and assign tmp to point to it
       head->next = (person*)kmalloc(sizeof(person), GFP_KERNEL);
       if(head->next == NULL){return;} // Error Checking
       tmp = head->next;

       // Assign values to the second node
       tmp->month = 10;
       tmp->day = 16;
       tmp->year = 1997;

       // Create the third node 
       tmp->next = (person*)kmalloc(sizeof(person), GFP_KERNEL);
       if(tmp->next == NULL){return;} // Error Checking
       tmp = tmp->next;

       // Assigns values to the third node
       tmp->month = 12;
       tmp->day = 15;
       tmp->year = 1989;

       // Create the fourth node
       tmp->next = (person*)kmalloc(sizeof(person), GFP_KERNEL);
       if(tmp->next == NULL){return;} // Error Checking
       tmp = tmp->next;

       // Assigns values to the fourth node
       tmp->month = 5;
       tmp->day = 22;
       tmp->year = 2002;

       // Create the fifth node
       tmp->next = (person*)kmalloc(sizeof(person), GFP_KERNEL);
       if(tmp->next == NULL){return;} // Error Checking
       tmp = tmp->next;

       // Assign values to the fifth node
       tmp->month = 1;
       tmp->day = 20;
       tmp->year = 2002;

       // End of the list
       tmp->next = NULL;

       return;
}


/*
Function:    printList()
Inputs:      None
Outputs:     None
Description: Prints a list to the kernel terminal
*/
void printList(){
       // Traversing pointer
       person *tmp = head;

       // If the head pointer is NULL, print empty list
       if(tmp == NULL){
              printk(KERN_INFO "Empty List\n");
       }

       // Keep printing the values in the linked list until the end
       while(tmp != NULL){
              printk(KERN_INFO "Month %d, Day %d, Year %d\n", tmp->month, tmp->day, tmp->year);

              tmp = tmp->next;
       }

       return;
}


/*
Function:    deleteList()
Inputs:      person *cur - pointer to a person node in a linked list
Outputs:     None
Description: Recursively deletes a linked list
*/
void deleteList(person *cur){
       // Stop case, once the end is reached, return
       if(cur == NULL)
              return;

       // Traverse to the end of the list
       deleteList(cur->next);

       // Delete from back to front
       kfree(cur);

       return;
}


/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

