// a few allocations in multiples of 4 bytes followed by frees
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"

int main() {

   assert(myInit(4096) == 0);
   void* rest = myAlloc(3908);

   void* ptr[8];
//dispMem();
   ptr[0] = myAlloc(8);
   ptr[1] = myAlloc(8);
   ptr[2] = myAlloc(8);
   ptr[3] = myAlloc(8);
   ptr[4] = myAlloc(16);
   ptr[5] = myAlloc(16);
   ptr[6] = myAlloc(24);
//dispMem();
   ptr[7] = myAlloc(24);
dispMem();
   assert(myAlloc(40) == NULL);
//dispMem();

   assert(myFree(ptr[4]) == 0);
   assert(myFree(ptr[5]) == 0);
   assert(myFree(ptr[6]) == 0);
//dispMem();

	void* fits = myAlloc(20);
	assert(fits != NULL);
//dispMem();

   exit(0);
}
