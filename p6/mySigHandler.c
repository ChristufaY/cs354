////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c, sendsig.c, division.c
// This File:        mySigHandler.c
// Other Files:      sendsig.c, division.c
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

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <time.h>

void handler_SIGALRM(){
    time_t now;
    now = time(NULL);
    if(now != -1 && ctime(&now) != NULL)
        printf("PID: %i CURRENT TIME: %s", getpid(), ctime(&now));
    else
	printf("PID: %i, CURRENT TIME: INVALID", getpid());
    alarm(3);
}

int counter_SIGUSR1 = 0;

void handler_SIGUSR1() {
    printf("SIGUSR1 handled and counted!\n");
    counter_SIGUSR1++;
}

void handler_SIGINT() {
    printf("\n");
    printf("SIGINT handled.\n");
    printf("SIGUSR1 was handled %i times. Exiting now.\n", counter_SIGUSR1);
    exit(0);
}
int main(){

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_flags = 0;
    sa.sa_handler = handler_SIGALRM;

    if(sigaction(SIGALRM, &sa, NULL) != 0){
        printf("Failed to bind SIGALRM handler.\n");
        exit(1);
    }

    struct sigaction su1;
    memset(&su1, 0, sizeof(su1));
    su1.sa_flags = 0;
    su1.sa_handler = handler_SIGUSR1;

    if(sigaction(SIGUSR1, &su1, NULL) != 0){
        printf("Failed to bind SIGUSR1 handler.\n");
        exit(1);
    }

    struct sigaction si;
    memset(&si, 0, sizeof(si));
    si.sa_flags = 0;
    si.sa_handler = handler_SIGINT;

    if(sigaction(SIGINT, &si, NULL) != 0) {
	printf("Failed to bind SIGINT handler.\n");
	exit(1);
    }

    printf("PID and time print every 3 seconds.\n");
    printf("Type Ctrl-C to end the program.\n");

    alarm(3);

    while(1);
    return 0;
}
