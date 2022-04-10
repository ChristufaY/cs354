////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c, sendsig.c, division.c
// This File:        division.c
// Other Files:      sendsig.c, mySigHandler.c
//
// Semester:         CS 354 Fall 2021
// Instructor:       Deppeler
//
// Author:           Christopher Yang
// Email:            cyang397@wisc.edu
// CS Login:         chrisy
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Dad helped with syntax and indentation
//
//
//
// Online sources:   Avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013,2019-2020
// Posting or sharing this file is prohibited, including any changes/additions.
//
////////////////////////////////////////////////////////////////////////////////
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int success = 0;

void handler_SIGFPE() {
    printf("Error: a division by 0 operation was attempted.\n");
    printf("Total number of operations completed successfully: %i\n", success);
    printf("The program will be terminated.\n");
    exit(0);
}

void handler_SIGINT() {
    printf("\n");
    printf("Total number of operations completed successfully: %i\n", success);
    printf("The program will be terminated.\n");
    exit(0);
}

int main(int argc, char *argv[]) {


    char strFirst[100];
    char strSecond[100];
    int first = 0;
    int second = 0;
    int result = 0;
    int remainder = 0;

    struct sigaction sfpe;
    memset(&sfpe, 0, sizeof(sfpe));
    sfpe.sa_flags = 0;
    sfpe.sa_handler = handler_SIGFPE;

    if(sigaction(SIGFPE, &sfpe, NULL) != 0) {
        printf("Error binding SIGFPE handler.\n");
        exit(1);
    }

    struct sigaction si;
    memset(&si, 0, sizeof(si));
    si.sa_flags = 0;
    si.sa_handler = handler_SIGINT;

    if(sigaction(SIGINT, &si, NULL) != 0) {
	printf("Error binding SIGINT handler.\n");
	exit(1);
    }

    while(1) {
        printf("Enter first integer: ");
        if(fgets(strFirst, 100, stdin) != NULL)
            first = atoi(strFirst);
	else {
	    printf("fgets failed.");
	    exit(0);
	}

	printf("Enter second integer: ");
        if(fgets(strSecond, 100, stdin) != NULL)
            second = atoi(strSecond);
	else {
	    printf("fgets failed.");
            exit(0);
        }

        result = first / second;
        remainder = first % second;
        printf("%i / %i is %i with a remainder of %i\n", first, second, result, remainder);
        success++;
    }
    return 0;
}

