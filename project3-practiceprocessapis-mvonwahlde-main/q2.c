/*
Author:  Matthew VonWahlde
Class:   CPSC 346 02
Date:    10/5/23
Project: Project 3, Question 2

Question:

2. Write another program using fork(). The child process shouldprint “hello”; 
the parent process should print “goodbye”. You should try to ensure that the 
child process always prints first; can you do this without calling wait() in 
the parent?

Answer:

Yes, a signal can be used instead.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

// Creating a type pid_t for process ID's
typedef int pid_t;

// Function prototypes
pid_t Fork(void);
void unix_error(char *msg);
void signal_handler(int dontCare);


// Main function, no input arguments
int main(void){
    
    // Create a new process
    if(Fork() == 0){
        // Child process

        // Print Hello and then wake up parent
        printf("Hello\n");
        kill(getppid(), SIGINT);
        exit(0);
    } 
    // Parent process

    // Set signal interrupt handler
    signal(SIGINT, signal_handler);

    // Wait for signal
    pause();
    printf("Goodbye\n");
    
    // Reap child after
    wait(NULL);

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


/*
Function:    signal_handler()
Inputs:      int - dontCare: Not used
Output:      None
Description: Handler for signals
*/
void signal_handler(int dontCare){
    return;
}
