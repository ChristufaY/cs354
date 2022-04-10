// a few allocations in multiples of 4 bytes
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"

int main() {
   assert(myInit(4096) == 0);
   assert(myAlloc(4) != NULL);
   assert(myAlloc(8) != NULL);
   assert(myAlloc(16) != NULL);
   assert(myAlloc(24) != NULL);
   dispMem();
   exit(0);
}
