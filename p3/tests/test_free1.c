// a few allocations in multiples of 4 bytes followed by frees
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"

int main() {
   assert(myInit(4096) == 0);
   void* ptr[4];

   ptr[0] = myAlloc(4);
   ptr[1] = myAlloc(8);
   dispMem();
   assert(myFree(ptr[0]) == 0);
   assert(myFree(ptr[1]) == 0);
   dispMem();
   ptr[2] = myAlloc(16);
   ptr[3] = myAlloc(4);
   dispMem();
   assert(myFree(ptr[2]) == 0);
   assert(myFree(ptr[3]) == 0);
   dispMem();
   exit(0);
}
