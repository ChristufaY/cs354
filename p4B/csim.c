////////////////////////////////////////////////////////////////////////////////
// Main File:        csim.c
// This File:        csim.c
// Other Files:      Makefile, trace files for testing
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
// Persons:          Dad helped with mostly higher level logistics of making
//                   loops, variables, if statements. Also helped debug.
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

/*
 * csim.c:
 * A cache simulator that can replay traces (from Valgrind) and output
 * statistics for the number of hits, misses, and evictions.
 * The replacement policy is LRU.
 *
 * Implementation and assumptions:
 *  1. Each load/store can cause at most 1 cache miss plus a possible eviction.
 *  2. Instruction loads (I) are ignored.
 *  3. Data modify (M) is treated as a load followed by a store to the same
 *  address. Hence, an M operation can result in two cache hits, or a miss and a
 *  hit plus a possible eviction.
 */

#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

/******************************************************************************/
/* DO NOT MODIFY THESE VARIABLES **********************************************/

//Globals set by command line args.
int b = 0; //number of block (b) bits
int s = 0; //number of set (s) bits
int E = 0; //number of lines per set

//Globals derived from command line args.
int B; //block size in bytes: B = 2^b
int S; //number of sets: S = 2^s

//Global counters to track cache statistics in access_data().
int hit_cnt = 0;
int miss_cnt = 0;
int evict_cnt = 0;

//Global to control trace output
int verbosity = 0; //print trace if set
/******************************************************************************/

//Type mem_addr_t: Use when dealing with addresses or address masks.
typedef unsigned long long int mem_addr_t;

//Type cache_line_t: Use when dealing with cache lines.
//TODO - COMPLETE THIS TYPE
typedef struct cache_line {
    char valid;
    mem_addr_t tag;
    int counter;
    //Add a data member as needed by your implementation for LRU tracking.
} cache_line_t;

//Type cache_set_t: Use when dealing with cache sets
//Note: Each set is a pointer to a heap array of one or more cache lines.
typedef cache_line_t* cache_set_t;
//Type cache_t: Use when dealing with the cache.
//Note: A cache is a pointer to a heap array of one or more sets.
typedef cache_set_t* cache_t;

// Create the cache (i.e., pointer var) we're simulating.
cache_t cache;

/* TODO - COMPLETE THIS FUNCTION
 * init_cache:
 * Allocates the data structure for a cache with S sets and E lines per set.
 * Initializes all valid bits and tags with 0s.
 */
void init_cache() {
    S = pow(2, s);
    B = pow(2, b);
    //printf("S: %d", S);
    //printf("B: %d", B);
    // initialize to 0 and NULL check "2D array" of cache size
    // # of sets times # of linees per set
    cache = malloc(sizeof(cache_set_t) * S);
    if(cache == NULL) {
	printf("Could not allocate %d sets.", S);
	exit(1);
    }
    for(int i = 0; i < S; i++) {
	cache[i] = malloc(sizeof(cache_line_t) * E);
        if(cache == NULL) {
	    printf("Could not allocate %d lines.", E);
	    exit(1);
        }
        for(int j = 0; j < E; j++) {
	    cache[i][j].valid = 0;
	    cache[i][j].tag = 0;
	}
    }
}
  

/* TODO - COMPLETE THIS FUNCTION 
 * free_cache:
 * Frees all heap allocated memory used by the cache.
 */                    
void free_cache() {             
    for(int i = 0; i < S; i++)
	free(cache[i]);
    free(cache);
}
   
   
/* TODO - COMPLETE THIS FUNCTION 
 * access_data:
 * Simulates data access at given "addr" memory address in the cache.
 *
 * If already in cache, increment hit_cnt
 * If not in cache, cache it (set tag), increment miss_cnt
 * If a line is evicted, increment evict_cnt
 */                    
void access_data(mem_addr_t addr) {
    // set number the address is looking for
    int setNum;
    // masking is used to "filter" bit that you want
    // set mask to all 1 bits to whatever bit size address
    mem_addr_t mask = 0xffffffffffffffff;
    // mask for s-bits
    mask = 0xffffffffffffffff;
    // shift all but b and s bit amount
    mask = mask >> (64 - b - s);
    // masking sets all but b and s bits to zero
    setNum = addr & mask;
    // shift to get rid of b-bits we dont need
    setNum = setNum >> b;
    // tag
    // shift b + s bits of the address to get the remaining bits, which
    // are the tag bits
    mem_addr_t currTag = addr >> (b + s);
    //printf("tag: %llu\n", currTag);
    // already in cache, increment hit_cnt
    int hit = 0;
    int miss = 0;
    // set min and max temps to extreme values in case of large "test
    // curr keeps track of current min or max counter value
    int currMax = -999999;
    int currMin = 1000000000;
    // keeps track of index of the cache line in the set that has min and max
    // counter values
    int currMaxLine = -1;
    int currMinLine = 100;
    // determines hit or miss
    for(int j = 0; j < E; j++) {
	if(cache[setNum][j].tag == currTag) {
	    if(cache[setNum][j].valid == 1) {
		hit = 1;
		currMaxLine = j;
	    } else {
		miss = 1;
	    }
	}
        // ? : is another way of writing an if statement
        currMax = cache[setNum][j].counter > currMax ?
            cache[setNum][j].counter : currMax;
	if(currMin > cache[setNum][j].counter) {
	    currMin = cache[setNum][j].counter;
	    currMinLine = j;
	}
    }
    if(!hit)
	miss = 1;
    if(hit) {
	if(verbosity)
	    printf("hit ");
	hit_cnt++;
	// currMax + 1 because this one is the "newest" line in the set
	// Most recently used 
        cache[setNum][currMaxLine].counter = currMax + 1;
    }
    int evict = 1;
    if(miss) {
	if(verbosity)
	    printf("miss ");
        miss_cnt++;
        // not in cache, cache it (set tag), increment miss_cnt
        for(int j = 0; j < E; j++) {
	    // check both tag and validity since 0 is a valid tag value
	    if(cache[setNum][j].tag == 0 && cache[setNum][j].valid == 0) {
	        cache[setNum][j].tag = currTag;
		cache[setNum][j].valid = 1;
		cache[setNum][j].counter = currMax + 1;
	        evict = 0;
	    }
        }
	if(evict) {
	    if(verbosity)
		printf("eviction ");
            cache[setNum][currMinLine].tag = currTag;
	    cache[setNum][currMinLine].valid = 1;
	    cache[setNum][currMinLine].counter = currMax + 1;
            evict_cnt++;
	}
    }
}



/* TODO - FILL IN THE MISSING CODE
 * replay_trace:
 * Replays the given trace file against the cache.
 *
 * Reads the input trace file line by line.
 * Extracts the type of each memory access : L/S/M
 * TRANSLATE each "L" as a load i.e. 1 memory access
 * TRANSLATE each "S" as a store i.e. 1 memory access
 * TRANSLATE each "M" as a load followed by a store i.e. 2 memory accesses 
 */                    
void replay_trace(char* trace_fn) {           
    char buf[1000];  
    mem_addr_t addr = 0;
    unsigned int len = 0;
    FILE* trace_fp = fopen(trace_fn, "r"); 

    if (!trace_fp) { 
        fprintf(stderr, "%s: %s\n", trace_fn, strerror(errno));
        exit(1);   
    }

    while (fgets(buf, 1000, trace_fp) != NULL) {
        if (buf[1] == 'S' || buf[1] == 'L' || buf[1] == 'M') {
            sscanf(buf+3, "%llx,%u", &addr, &len);
      
            if (verbosity)
                printf("%c %llx,%u ", buf[1], addr, len);

            // TODO - MISSING CODE
            // GIVEN: 1. addr has the address to be accessed
            //        2. buf[1] has type of acccess(S/L/M)
            // call access_data function here depending on type of access
	    if(buf[1] == 'S' || buf[1] == 'L')
		access_data(addr);
	    // access_data is called twice because modify is a load and a store
	    if(buf[1] == 'M') {
		access_data(addr);
		access_data(addr);
	    }
            if (verbosity)
                printf("\n");
        }
    }

    fclose(trace_fp);
}  
  
  
/*
 * print_usage:
 * Print information on how to use csim to standard output.
 */                    
void print_usage(char* argv[]) {                 
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of s bits for set index.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of b bits for block offsets.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}  
  
  
/*
 * print_summary:
 * Prints a summary of the cache simulation statistics to a file.
 */                    
void print_summary(int hits, int misses, int evictions) {                
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
    FILE* output_fp = fopen(".csim_results", "w");
    assert(output_fp);
    fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
    fclose(output_fp);
}  
  
  
/*
 * main:
 * Main parses command line args, makes the cache, replays the memory accesses
 * free the cache and print the summary statistics.  
 */                    
int main(int argc, char* argv[]) {                      
    char* trace_file = NULL;
    char c;
    
    // Parse the command line arguments: -h, -v, -s, -E, -b, -t 
    while ((c = getopt(argc, argv, "s:E:b:t:vh")) != -1) {
        switch (c) {
            case 'b':
                b = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'h':
                print_usage(argv);
                exit(0);
            case 's':
                s = atoi(optarg);
                break;
            case 't':
                trace_file = optarg;
                break;
            case 'v':
                verbosity = 1;
                break;
            default:
                print_usage(argv);
                exit(1);
        }
    }

    //Make sure that all required command line args were specified.
    if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]);
        print_usage(argv);
        exit(1);
    }

    //Initialize cache.
    init_cache();

    //Replay the memory access trace.
    replay_trace(trace_file);

    //Free memory allocated for cache.
    free_cache();

    //Print the statistics to a file.
    //DO NOT REMOVE: This function must be called for test_csim to work.
    print_summary(hit_cnt, miss_cnt, evict_cnt);
    return 0;   
}  





// 202109
