// check for coalesce free space
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "myHeap.h"

int main() {

   int size = 1020;

   assert(myInit(4096) == 0);
   void * ptr[15];
   ptr[0] = myAlloc(size);

   int i=1;
   while ( ptr[i-1] != NULL ) {
	   ptr[i++] = myAlloc(size);
   }

   dispMem();
   int result = myFree(ptr[1]);
   result = myFree(ptr[2]);

   dispMem();

   ptr[1] = myAlloc(size*2);
   assert(ptr[1] == NULL);

   assert(coalesce()>0);

   dispMem();

   ptr[1] = myAlloc(size*2);
   assert(ptr[1] != NULL);
   dispMem();

   exit(0);
}
