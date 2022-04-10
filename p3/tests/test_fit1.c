// a few allocations in multiples of 4 bytes followed by frees
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"

int main() {
   assert(myInit(4096) == 0);
   void* ptr[8];

   ptr[0] = myAlloc(4);
   ptr[1] = myAlloc(4);
   ptr[2] = myAlloc(8);
   ptr[3] = myAlloc(8);
   ptr[4] = myAlloc(16);
   ptr[5] = myAlloc(16);
   ptr[6] = myAlloc(24);
   ptr[7] = myAlloc(24);
   dispMem();
   assert(myFree(ptr[0]) == 0);
   assert(myFree(ptr[3]) == 0);
   assert(myFree(ptr[5]) == 0);
   assert(myFree(ptr[6]) == 0);
   dispMem();
   ptr[3] = myAlloc(8);
   assert(ptr[3] < ptr[4]);
   dispMem();
   ptr[0] = myAlloc(4);
   assert(ptr[0] < ptr[1]);
   dispMem();
   exit(0);
}
