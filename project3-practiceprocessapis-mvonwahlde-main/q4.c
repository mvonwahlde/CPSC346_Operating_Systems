/*
Author:  Matthew VonWahlde
Class:   CPSC 346 02
Date:    10/5/23
Project: Project 3, Question 4

Question:

4. Now write a program that uses wait() in the parent to wait for the child process 
to finish. What does wait() return? What happens if you use wait() in the child?

Answer:

Wait returns the pid for the process that finished executing and the status is put in 
an inputted pointer. If wait is called in the child process, wait returns -1 immediately
with a status of 0.
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

    // Create a new process
    if(Fork() == 0){
        // Child process

        printf("Child Running\n");

        // Child calls wait
        int status;
        pid_t pid = wait(&status);

        // Print out the return status and pid of the wait call in the child
        printf("Wait Status from Child: %d\n", status);
        printf("Wait Return from Child: %d\n", pid);

        printf("Child Finished\n");
        exit(0);
    } 
    // Parent process

    // Wait for child to exit and reap them
    wait(NULL);
    printf("Parent Running and Finishing\n");

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
