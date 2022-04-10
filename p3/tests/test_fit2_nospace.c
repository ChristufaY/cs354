// many odd sized allocations and interspersed frees
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"

int main() {
   assert(myInit(4096) == 0);
	void *ptr[9];
   ptr[0] = myAlloc(1);
   ptr[1] = (myAlloc(5));
   ptr[2] = (myAlloc(8));
   ptr[3] = (myAlloc(18));
   ptr[4] = (myAlloc(80));

   assert(ptr[0] != NULL);
   assert(ptr[1] != NULL);
   assert(ptr[2] != NULL);
   assert(ptr[3] != NULL);
   assert(ptr[4] != NULL);
   
   assert(myFree(ptr[1]) == 0);
   assert(myFree(ptr[0]) == 0);
   assert(myFree(ptr[3]) == 0);
   
   assert(myAlloc(13) < ptr[4]);
   assert(myAlloc(1) < ptr[1]);
   
   assert((ptr[5] = myAlloc(4)) < ptr[2]);
   //ptr[5] = myAlloc(4);
   //dispMem();
   assert((ptr[6] = myAlloc(200)) > ptr[4]);
   //dispMem();
   assert((ptr[7] = myAlloc(300)) > ptr[6]);
   //dispMem();
   assert((ptr[8] = myAlloc(400)) > ptr[7]);
   //dispMem();
   assert(myFree(ptr[8]) == 0);
   assert(myFree(ptr[6]) == 0);
   assert(myFree(ptr[7]) == 0);

   assert((ptr[8] = myAlloc(400)) > ptr[7]);
   dispMem();
   exit(0);
}
