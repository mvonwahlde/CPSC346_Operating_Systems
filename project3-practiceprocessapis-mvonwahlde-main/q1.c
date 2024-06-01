/*
Author:  Matthew VonWahlde
Class:   CPSC 346 02
Date:    10/5/23
Project: Project 3, Question 1

Question:

1. Write a program that calls fork(). Before calling fork(), have the main process
access a variable (e.g., x) and set its value to some-thing (e.g., 100). What value 
is the variable in the child process? What happens to the variable when both the 
child and parent change the value of x?

Answer:

The variable in the child process is set to 100.
The variable in the parent process is set to 101.
When the child and parent modify the variable, it is modified in their own seperate
processes. So, the variable has different values in each process.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// Creating a type pid_t for process ID's
typedef int pid_t;

// Function prototypes
pid_t Fork(void);
void unix_error(char *msg);


// Main function, no input arguments
int main(void){
    int x;

    // Create a new process
    if(Fork() == 0){
        // Child process
        printf("Child: x = %d\n", x = 100);
        exit(0);
    } 
    // Parent process

    // Wait for child to exit and reap them
    wait(NULL);
    printf("Parent: x = %d\n", x = 101);

    return 0;
}


/*
Function:   Fork()
Inputs:     None
Output:     pid_t - process id (pid): child will return 0, parent will return the pid of the child
Decription: Wrapper function for fork() to handle error checking
*/
pid_t Fork(void){
    pid_t pid;

    if((pid = fork()) < 0){
        unix_error("fork error");
    }

    return pid;
}


/*
Function:    unix_error()
Inputs:      char* - msg: string to print out before printing error
Output:      None
Description: Prints out the unix error message using global errno
*/
void unix_error(char *msg){
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(-1);
}
