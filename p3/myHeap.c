///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2021 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission Fall 2021, CS354-deppeler
//
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Main File:        Files with main methods are in the test folder
// This File:        myHeap.c
// Other Files:      Makefile, myHeap.h, test directory containing all of the
//		     test files.
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
// Persons:          Dad helped with myAlloc, myFree, and coalesce. Specifically
//		     the higher level thinking of how to structure the program.
//
//
//
// Online sources:   Avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "myHeap.h"

/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {

    int size_status;
    /*
     * Size of the block is always a multiple of 8.
     * Size is stored in all block headers and in free block footers.
     *
     * Status is stored only in headers using the two least significant bits.
     *   Bit0 => least significant bit, last bit
     *   Bit0 == 0 => free block
     *   Bit0 == 1 => allocated block
     *
     *   Bit1 => second last bit
     *   Bit1 == 0 => previous block is free
     *   Bit1 == 1 => previous block is allocated
     *
     * End Mark:
     *  The end of the available memory is indicated using a size_status of 1.
     *
     * Examples:
     *
     * 1. Allocated block of size 24 bytes:
     *    Allocated Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 25
     *      If the previous block is allocated p-bit=1 size_status would be 27
     *
     * 2. Free block of size 24 bytes:
     *    Free Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 24
     *      If the previous block is allocated p-bit=1 size_status would be 26
     *    Free Block Footer:
     *      size_status should be 24
     */
} blockHeader;

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */
blockHeader *heapStart = NULL;
/* Size of heap allocation padded to round to nearest page size.
 */
int allocsize;

/*
 * Additional global variables may be added as needed below
 */


/*
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block (payload) on success.
 * Returns NULL on failure.
 *
 * This function must:
 * - Check size - Return NULL if not positive or if larger than heap space.
 * - Determine block size rounding up to a multiple of 8
 *   and possibly adding padding as a result.
 *
 * - Use BEST-FIT PLACEMENT POLICY to chose a free block
 *
 * - If the BEST-FIT block that is found is exact size match
 *   - 1. Update all heap blocks as needed for any affected blocks
 *   - 2. Return the address of the allocated block payload
 *
 * - If the BEST-FIT block that is found is large enough to split
 *   - 1. SPLIT the free block into two valid heap blocks:
 *         1. an allocated block
 *         2. a free block
 *         NOTE: both blocks must meet heap block requirements
 *       - Update all heap block header(s) and footer(s)
 *              as needed for any affected blocks.
 *   - 2. Return the address of the allocated block payload
 *
 * - If a BEST-FIT block found is NOT found, return NULL
 *   Return NULL unable to find and allocate block for desired size
 *
 * Note: payload address that is returned is NOT the address of the
 *       block header.  It is the address of the start of the
 *       available memory for the requesterr.
 *
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* myAlloc(int size) {
    if(size <= 0) {
	fprintf(stderr, "Error:myHeap.c: Requested size is not positive\n");
	return NULL;
    }
    int sizeNeeded = 4 + size;
    while(sizeNeeded % 8 != 0)
	 sizeNeeded++;
    blockHeader *current = heapStart;
    blockHeader *newAlloc;
    blockHeader *newFree;
    blockHeader *currBest = NULL;
    blockHeader *nextBLK;
    int t_size;
    int currBestSize = 1000000;
    int is_used   = -1;
    int p_is_used = -1;
    while (current->size_status != 1) {
	//printf("%08x\n", (unsigned int)current);
        t_size = current->size_status;

	if (t_size & 1) {
            // LSB = 1 => used block
            is_used = 1;
            t_size = t_size - 1;
        } else {
            is_used = 0;
        }

        if (t_size & 2) {
            t_size = t_size - 2;
	    p_is_used = 1;
        } else {
	    p_is_used = 0;
        }

        // is_used == true, then go to the next header; current + t_size
        if (is_used) {
            current = (blockHeader*)((char*)current + t_size);
            continue;
        }

	// split
	if(t_size > sizeNeeded) {
	    if(t_size < currBestSize) {
		currBestSize = t_size;
		currBest = current;
		current = (blockHeader*)((char*)current + t_size);
		//printf("1:%08x\n", (unsigned int)current);
		continue;
	    }
	}
	// exact match
	if(t_size == sizeNeeded) {
	    newAlloc = (blockHeader*)(current);
	    newAlloc->size_status = sizeNeeded;
            newAlloc->size_status += 1;
	    if(p_is_used)
		newAlloc->size_status += 2;
	    nextBLK = (blockHeader*)((char*)current + t_size);
	    if(nextBLK->size_status != 1)
        	nextBLK->size_status += 2;
	    return (void*)(newAlloc + 1);
	}
	current = (blockHeader*)((char*)current + t_size);
	//printf("2:%08x\n", (unsigned int)current);
    }
    if(currBest != NULL) {
        newAlloc = (blockHeader*)(currBest);
	p_is_used = 0;
	if(newAlloc->size_status & 2)
	    p_is_used = 1;
	// account for allocation bit (a-bit)
        newAlloc->size_status = sizeNeeded + 1;
        if(p_is_used)
            newAlloc->size_status += 2;
        newFree = (blockHeader*)((char*)currBest + sizeNeeded);
        //printf("%08x", (unsigned int)(current));
        //printf("%08x", (unsigned int)(current + sizeNeeded));
	// account for the previous bit (p-bit)
        newFree->size_status = currBestSize - sizeNeeded + 2;
        // Set the footer
	// subtract by 4 to align within block of memory
        blockHeader *footer = (blockHeader*) ((char*)currBest + currBestSize - 4);
        footer->size_status = currBestSize - sizeNeeded;
	// add one to return the address of the payload not the header (4 bytes)
        return (void*)(newAlloc + 1);
    }
    return NULL;
}

/*
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - Update header(s) and footer as needed.
 */
int myFree(void *ptr) {
    //printf("ptr: %08x\n", (unsigned int)ptr);
    if(ptr == NULL) {
	fprintf(stderr, "Error:myHeap.c: Ptr is NULL.\n");
        return -1;
    }
    if(((int)ptr % 8) != 0) {
        fprintf(stderr, "Error:myHeap.c: Ptr is not a multiple of 8.\n");
        return -1;
    }
    //printf("ptr - 4: %08x\n", (unsigned int)ptr - 4);
    //printf("heapStart: %08x\n", (unsigned int)heapStart);
    //printf("heapStart + allocsize - 8: %08x\n", (unsigned int)heapStart + allocsize - 8);
    if((unsigned int)ptr - 4 < (unsigned int)heapStart ||
	(unsigned int)ptr - 4 > (unsigned int)heapStart + allocsize - 8) {
        fprintf(stderr, "Error:myHeap.c: Ptr is outside of the heap space.\n");
        return -1;
    }

    //blockHeader* current = (blockHeader*)((void*)ptr);
    //printf("current: %08x\n", (unsigned int)current);
    blockHeader* current = (blockHeader*)((char*)ptr - 4);
    //printf("current - 4: %08x\n", (unsigned int)current);
    //current = (blockHeader*)((void*)ptr - 1);
    //printf("current - 1: %08x\n", (unsigned int)current);
    int size = current->size_status;
    if(!(size & 1)) {
        fprintf(stderr, "Error:myHeap.c: Ptr is already freed.\n");
        return -1;
    }
    // get the size and keep track of previous allocation so that we can
    // account for it when we update the header for current
    int p_alloc = 0;
    // & with one to check the least significant bit (true if odd, false if even)
    if (size & 1)
        size = size - 1;
    // & with two to check the second least significant bit (true if the bit is 1,
    // false if the bit is 0)
    if (size & 2) {
        size = size - 2;
	p_alloc = 1;
    }
    // set the footer
    if(p_alloc) {
	blockHeader *footer = (blockHeader*) ((char*)current + size - 4);
	footer->size_status = size;
	current->size_status = size + 2;
    } else {
	blockHeader *footer = (blockHeader*) ((char*)current + size - 4);
	footer->size_status = size;
	current->size_status = size;
    }

    // go to the next block and set the p-bit to 0
    blockHeader* nextBLKH = (blockHeader*)((char*)current + size);
    // check if the next block is the end mark
    if(nextBLKH->size_status != 1)
	nextBLKH->size_status -= 2;

    return 0;
}

/*
 * Function for traversing heap block list and coalescing all adjacent
 * free blocks.
 *
 * This function is used for delayed coalescing.
 * Updated header size_status and footer size_status as needed.
 */
int coalesce() {
    blockHeader* current = (blockHeader*)heapStart;
    int currSize = 0;
    blockHeader* next;
    int nextSize = 0;
    int currIsUsed = -1;
    int nextIsUsed = -1;
    blockHeader* newFooter;
    int currPBit = 0;
    int merged = 0;
    int totalSize = 0;
    while (current->size_status != 1) {
	currSize = current->size_status;

        if (currSize & 1) {
            currIsUsed = 1;
            currSize -= 1;
        } else
            currIsUsed = 0;

        if (currSize & 2) {
            currSize -= 2;
	    currPBit = 1;
	} else
	    currPBit = 0;

	next = (blockHeader*)((char*)current + currSize);
	nextSize = next->size_status;
	if(nextSize == 1)
	    break;
	if (nextSize & 1) {
	    nextIsUsed = 1;
	    nextSize -= 1;
	} else
	    nextIsUsed = 0;

	if (nextSize & 2)
	    nextSize -= 2;
	totalSize += currSize + nextSize;
	//printf("Current %08x\n", (unsigned int)current);
	//printf("Next %08x\n", (unsigned int)next);
        // both current and next are free
	if (!currIsUsed && !nextIsUsed) {
            current->size_status = currSize + nextSize;
	    newFooter = (blockHeader*)((char*)current + currSize + nextSize - 4);
	    newFooter->size_status = currSize + nextSize;
	    if(currPBit)
		current->size_status += 2;
	    merged = 1;
	}
	// current is free, next is used
	else if (nextIsUsed)
	    current = (blockHeader*)((char*)current + currSize + nextSize);
	// current is used, next is free
	else if (currIsUsed && !nextIsUsed)
	    current = next;

    }

	return merged;
}


/*
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */
int myInit(int sizeOfRegion) {

    static int allocated_once = 0; //prevent multiple myInit calls

    int pagesize;   // page size
    int padsize;    // size of padding when heap size not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int fd;

    blockHeader* endMark;

    if (0 != allocated_once) {
        fprintf(stderr,
        "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }

    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    allocsize = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, allocsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }

    allocated_once = 1;

    // for double word alignment and end mark
    allocsize -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heapStart = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    endMark = (blockHeader*)((void*)heapStart + allocsize);
    endMark->size_status = 1;
    //printf("Endmark: %08x\n", (unsigned int)endMark);
    //printf("Endmark size_status: %d\n", endMark->size_status);
    // Set size in header
    heapStart->size_status = allocsize;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heapStart->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heapStart + allocsize - 4);
    footer->size_status = allocsize;

    return 0;
}
/*
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts)
 * t_End    : address of the last byte in the block
 * t_Size   : size of the block as stored in the block header
 */
void dispMem() {

    int counter;
    char status[6];
    char p_status[6];
    char *t_begin = NULL;
    char *t_end   = NULL;
    int t_size;

    blockHeader *current = heapStart;
    counter = 1;

    int used_size = 0;
    int free_size = 0;
    int is_used   = -1;

    fprintf(stdout,
	"*********************************** Block List **********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout,
	"---------------------------------------------------------------------------------\n");

    while (current->size_status != 1) {
	//printf("Current: %08x\n", (unsigned int)current);
	//printf("Current size_status: %d\n", current->size_status);
        t_begin = (char*)current;
        t_size = current->size_status;

        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "alloc");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "FREE ");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "alloc");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "FREE ");
        }

        if (is_used)
            used_size += t_size;
        else
            free_size += t_size;

        t_end = t_begin + t_size - 1;

        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%4i\n", counter, status,
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);

        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout,
	"---------------------------------------------------------------------------------\n");
    fprintf(stdout,
	"*********************************************************************************\n");
    fprintf(stdout, "Total used size = %4d\n", used_size);
    fprintf(stdout, "Total free size = %4d\n", free_size);
    fprintf(stdout, "Total size      = %4d\n", used_size + free_size);
    fprintf(stdout,
	"*********************************************************************************\n");
    fflush(stdout);

    return;
}


// end of myHeap.c (Fall 2021)
// dd


