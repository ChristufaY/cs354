////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c, sendsig.c, division.c
// This File:        sendsig.c
// Other Files:      mySigHandler.c, division.c
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    if(argc != 3) {
	printf("Usage: sendsig <signal type> <pid>\n");
	exit(0);
    }

    if(*(argv[1]+1) == 'i') {
	if(kill(atoi(argv[2]), 2) != 0)
	    printf("sendsig -i %s failed.\n", argv[2]);
    }
    else if(*(argv[1]+1) == 'u') {
	if(kill(atoi(argv[2]), 10) != 0)
	    printf("sendsig -u %s failed.\n", argv[2]);
    }
    else
	printf("Usage: sendsig <signal type> <pid>\n");

    return 0;
}
