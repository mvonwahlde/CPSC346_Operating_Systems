/*
Author:  Matthew VonWahlde
Class:   CPSC 346 02
Date:    10/5/23
Project: Project 3, Question 3

Question:

3. Write a program that calls fork() and then calls some form of exec() to run 
the program /bin/ls. Try at least one of the variants of exec(), including 
(on Linux) execl(), execle(),execlp(), execv(), execvp(), and execvpe(). Why do 
you think there are so many variants of the same basic call?

Answer:

There are many variants of the same call because there are other factors the user
may want to consider. For example, if the user wants or doesn't want to edit the 
environment variables or search the PATH.
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

        // Create arguments and execute ls
        char *argv[] = {"/bin/ls", NULL};
        execv(argv[0], argv);
        
        // Should not reach here
        exit(-1);
    } 
    // Parent process

    // Wait for child to exit and reap them
    wait(NULL);
    printf("Child reaped\n");

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
