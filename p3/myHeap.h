///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2021 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Fall 2021
///////////////////////////////////////////////////////////////////////////////

#ifndef __myHeap_h
#define __myHeap_h

int   myInit(int sizeOfRegion);
void* myAlloc(int size);
int   myFree(void *ptr);
int   coalesce();
void  dispMem();

void* malloc(size_t size) {
    return NULL;
}

#endif // __myHeap_h__

