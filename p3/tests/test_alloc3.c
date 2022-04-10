// many odd sized allocations
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"

int main() {
   assert(myInit(4096) == 0);
   assert(myAlloc(1) != NULL);
   assert(myAlloc(5) != NULL);
   assert(myAlloc(14) != NULL);
   assert(myAlloc(8) != NULL);
   assert(myAlloc(1) != NULL);
   assert(myAlloc(4) != NULL);
   assert(myAlloc(55) != NULL);
   assert(myAlloc(9) != NULL);
   assert(myAlloc(33) != NULL);
   dispMem();
   exit(0);
}
