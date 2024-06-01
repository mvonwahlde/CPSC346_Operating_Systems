/*
File:    buffer.h
Author:  Matthew VonWahlde
Date:    November 10, 2023
Class:   CPSC 346 02
Project: Project 7 - Producer Consumer
Description:
	Header file for buffer.c
*/

// Add neccessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// Change name from int to buffer_item
typedef int buffer_item;

#define BUFFER_SIZE                5   /* Maximum elements in the buffer                   */
#define TRUE                       1   /* TRUE is equivalent to 1 for boolean logic        */

// Comment out if you want random sleep time to be between 0 and MAX_INT
#define PRODUCER_MAX_SLEEP_TIME    4   /* Limit producer random sleeping time to 4 seconds */
#define CONSUMER_MAX_SLEEP_TIME    4   /* Limit consumer random sleeping time to 4 seconds */

// Function prototypes
int insert_item(buffer_item item);
int remove_item(buffer_item *item);
