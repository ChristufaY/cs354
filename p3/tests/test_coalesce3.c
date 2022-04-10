// check for coalesce free space
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"

int main() {
   assert(myInit(4096) == 0);
   void * ptr[5];

   ptr[0] = myAlloc(500);
   assert(ptr[0] != NULL);

   ptr[1] = myAlloc(500);
   assert(ptr[1] != NULL);

   ptr[2] = myAlloc(500);
   assert(ptr[2] != NULL);

   ptr[3] = myAlloc(500);
   assert(ptr[3] != NULL);

   ptr[4] = myAlloc(500);
   assert(ptr[4] != NULL);

   while (myAlloc(500) != NULL)
     ;

   assert(myFree(ptr[1]) == 0);
   assert(myFree(ptr[3]) == 0);
   assert(myFree(ptr[2]) == 0);

   ptr[3] = myAlloc(1300);
   assert(ptr[3] == NULL);
dispMem();
   coalesce();
dispMem();
   ptr[3] = myAlloc(1300);
   assert(ptr[3] != NULL);

   exit(0);
}
