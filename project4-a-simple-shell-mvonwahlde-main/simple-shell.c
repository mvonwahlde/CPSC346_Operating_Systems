/*
Author:      Matthew VonWahlde
Date:        10/15/2023
Class:       CPSC 346 02
Project:     4 - A simple shell
Description: Create a simple shell in the terminal that can run commands in both the foreground and
		     background. In addition, the shell should have history functionality, storing the previous
			 9 commands.
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>


#define MAX_LINE		80     /* 80 chars per line, per command, should be enough. */
#define MAX_COMMANDS	9      /* size of history */

// Function prototypes
void addtohistory(char inputBuffer[]);
int setup(char inputBuffer[], char *args[],int *background, int *nonFatalError);
pid_t Fork(void);
void unix_error(char *msg);
void dPrintf(char arr[]);


char history[MAX_COMMANDS][MAX_LINE];            // Array to store previous commands in the history
char display_history [MAX_COMMANDS][MAX_LINE];   // Previous commnands formatted for printf

int command_count = 0; // Count for the total number of commands executed


// Main Function
int main(void) {
	char inputBuffer[MAX_LINE]; // buffer to hold the command entered
	int background;             // equals 1 if a command is followed by '&'
	char *args[MAX_LINE/2 + 1];	// command line (of 80) has max of 40 arguments
	pid_t child;            	// process id of the child process
	int shouldrun = 1;          // Flag to keep looping
	int nonFatalError = 0;      // Flag to check if a command should be executed

	// Main Loop
    while (shouldrun){            	
		// Set background to 0 initially
		background = 0;
		nonFatalError = 0;

		// Reset the input buffer 
		for(int i = 0; i < MAX_LINE; i++)
			inputBuffer[i] = 0;

		// Check if there are any children to reap (from running in background)
		waitpid(-1, NULL, WNOHANG);

		// Grab user input and parse
		shouldrun = setup(inputBuffer,args,&background,&nonFatalError);

		// If the user typed in "exit", exit program
		if(strncmp(inputBuffer, "exit", 4) == 0){
			printf("\n"); // For terminal formatting purposes
			return 0;
		}
		
		// If the user typed in "history", show the previous commands (up to 9)
		if(strncmp(inputBuffer, "history", 7) == 0){
			// Check to make sure there are commands
			if(command_count == 0){
				printf("No commands in history\n");
			} else {
				// If there are commands

				// Determine the number of commands to display (max of 9)
				int lines = MAX_COMMANDS;
				if(command_count < MAX_COMMANDS)
					lines = command_count;
				for(int i = 0; i < lines; i++){
					// For each command, display what is in the display history
					printf("%d \t", lines - i);
					display_history[(command_count - i - 1) % 9][MAX_LINE-1] = '\0';
					printf("%s", display_history[(command_count - i - 1) % 9]);
					printf("\n");
					// Making sure to flush the buffer
					fflush(stdout);
				}
			}
			// Skip the command execution phase
			continue;
		}
		
		// Checkk if shouldrun is 1 and there was not a nonfatal error
		if (shouldrun && !nonFatalError) {
			// Create another process
			if((child = Fork()) == 0){
				// Child process

				// Run the commands and arguments inputted
				execvp(args[0], args);
				unix_error("Error - execvp failed");
				exit(-1);
			}
			// Parent process
			if(background == 0){
				// If running in the foreground
				waitpid(child, NULL, 0);
			}
		}
    }
	
	return 0;
}


/*
Function:    addtohistory()
Inputs:      char inputBuffer[] - Buffer of length MAX_COMMANDS to store in the history
Outputs:     None
Description: Adds the inputted command to the history, then formats and adds to the display_history
*/
void addtohistory(char inputBuffer[]) {
	// Increment the global command count and then calculate the array row to store in
	command_count++;
	int line = (command_count-1) % MAX_COMMANDS;
	
	// Copy the input buffer to the appropriate line in history	
	strcpy(history[line], inputBuffer);

	// Format and add to display_history
	int newLineReached = 0; // If new line reached, everything after is irrelevant
	for(int i = 0; i < MAX_LINE; i++){
		// Get the current character
		char cur = history[line][i];
		
		// If the '\n' has already been reached, fill the rest with spaces
		if(newLineReached == 1)
			cur = ' ';

		// If special character, fill in with space
		if(cur == '\0' || cur == '\n'){
			display_history[line][i] = ' ';
			
			// If there is a new line, set flag
			if(cur == '\n')
				newLineReached = 1;
		} else {
			// Character is good to be added to the display history
			display_history[line][i] = cur;
		}
	}

	return;
}


/*
Function:    setup()
Inputs:      char inputBuffer[] - Buffer of length MAX_COMMANDS to store command
			 char *args[]       - Argument array for execvp
			 int *background    - (Return) set to 1 if command should execute in the background
			 int *nonFatalError - (Return) set to 1 if there was an error that should not kill the shell
Outputs:     Return 0 if the shell should end
Description: Reads and formats commands from user input
*/
int setup(char inputBuffer[], char *args[], int *background, int *nonFatalError) {
    int length,		// Number of characters in the command line
	i,				// Loop index for accessing inputBuffer array
	command_number;	// Number of commands in the 
	
    // Read what the user enters on the command line
	do {
		printf("osh>");
		fflush(stdout);
		length = read(STDIN_FILENO,inputBuffer,MAX_LINE); 
	}
	while (inputBuffer[0] == '\n'); // Swallow newline characters

	// If ^d was entered, end of user command stream
    if (length == 0){
        printf("\n");
		exit(0);          
	}

	// If there was a read() error, terminate program with exit code 1
    if ( (length < 0) ) {
		perror("Error reading the command\n");
		exit(-1);           
    }

	// Check to see if the user is using commands from history
	int lineNum = -1;
	if (inputBuffer[0] == '!') {
		// If there are no commands in the history, print and return (non fatal error)
		if(command_count == 0){
			printf("No commands in history\n");
			*nonFatalError = 1;
			return 1;
		}
		// Check if the user wants to execute a previous command
		if(inputBuffer[1] == '!'){
			// The last command
			// Set the line number to the previous command
			lineNum = (command_count - 1) % MAX_COMMANDS;
		} else if(isdigit(inputBuffer[1]) && !isdigit(inputBuffer[2])){
			// Check if the number entered was a valid command number
			int num = inputBuffer[1] - 48;
			if(num > command_count || num == 0){
				// If there is no command, exit with non fatal error
				printf("No such command in history\n");
				*nonFatalError = 1;
				return 1;
			} else {
				// Caculate the line number in the history array
				if(command_count < 10)
					lineNum = num - 1;
				else
					lineNum = (command_count + num - 1) % 9;
			}
		} else {
			// Catch any other cases with a non fatal error
			printf("No such command in history\n");
			*nonFatalError = 1;
			return 0;
		}
	}
	
	// If lineNum was set, run the command from the history
	if(lineNum > -1){
		// Copy into the input buffer and calculate the length of the command
		strcpy(inputBuffer, history[lineNum]);
		length = 0;
		while(inputBuffer[length++] != '\n' && length < MAX_LINE);
	}
	
	// If the command was not "history", add the command to the history
	if( !(strncmp(inputBuffer, "history", 7) == 0) )
		addtohistory(inputBuffer); 
	
	// Parse the inputBuffer and set the args array
	int nextCharStart = 1; // Flag, set if the next character is the start of a new argument
	command_number = 0;
    for (i = 0; i < length; i++) { 
		// Examine every character in the inputBuffer
        switch (inputBuffer[i]){
			case ' ':
				// If space, next command will be the start of an argument
				nextCharStart = 1;
				inputBuffer[i] = '\0';

			case '\t' : 				
				// If tab, next command will be the start of an argument
				nextCharStart = 1;
				inputBuffer[i] = '\0';
				
			case '\n':  
				// End of user input, set the last element in args to NULL
				inputBuffer[i] = '\0';
				args[command_number] = NULL;
				break;
				
	    	default :             
				// Every other character

				// If &, set the background to 1
				if(inputBuffer[i] == '&') {
					*background = 1;
				} else if(nextCharStart == 1) {
					// If the start of new command, enter the address into args
					nextCharStart = 0;
					args[command_number] = &inputBuffer[i];
					command_number++;
				}
		}
	}
	
	// Eliminate any remaining & symbols until new line is reached
	for(int c = 0; c < command_number; c++) {
		int l = 0;
		while(args[c][l] != '\n') {
			if(args[c][l] == '&') {
				args[c][l] = ' ';
			}
			l++;
		}
	}

	return 1;	
}


/*
Function:   Fork()
Inputs:     None
Output:     pid_t - process id (pid): child will return 0, parent will return the pid of the child
Decription: Wrapper function for fork() to handle error checking
*/
pid_t Fork(void) {
    pid_t pid;

    if((pid = fork()) < 0) {
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
void unix_error(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(-1);
}


/*
Function:    dPrintf() (DEBUG FUNCTION)
Inputs:      char arr[]: string to print out
Output:      None
Description: Prints out the inputted array, special characters' ASCII value are printed out
*/
void dPrintf(char arr[]) {
	// Go through each element in the array
	for(int i = 0; i < MAX_LINE; i++){
		// If letter or space, print out character
		if(arr[i] >= 97 && arr[i] <= 122 || arr[i] == 32)
			printf("%c", arr[i]);
		else 
			// Otherwise, print ASCII value
			printf(" %d ", (int)arr[i]);
	}
	printf("\n");
}
