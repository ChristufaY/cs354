// check for coalesce free space
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"

int main() {
   assert(myInit(4096) == 0);
   void * ptr[4];

   ptr[0] = myAlloc(800);
   assert(ptr[0] != NULL);

   ptr[1] = myAlloc(800);
   assert(ptr[1] != NULL);

   ptr[2] = myAlloc(800);
   assert(ptr[2] != NULL);

   ptr[3] = myAlloc(800);
   assert(ptr[3] != NULL);

   while (myAlloc(800) != NULL)
     ;

   assert(myFree(ptr[1]) == 0);
   assert(myFree(ptr[2]) == 0);
   assert(myFree(ptr[0]) == 0);

   ptr[2] = myAlloc(1200);
   assert(ptr[2] == NULL);
dispMem();
   coalesce();
dispMem();
   ptr[2] = myAlloc(1600);
   assert(ptr[2] != NULL);

   exit(0);
}
