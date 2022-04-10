// many odd sized allocations checked for alignment
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include "myHeap.h"

int main() {
    assert(myInit(4096) == 0);
    void* ptr[9];
    ptr[0] = myAlloc(1);
    ptr[1] = myAlloc(14);
    ptr[2] = myAlloc(33);
    ptr[3] = myAlloc(8);
    ptr[4] = myAlloc(1);
    ptr[5] = myAlloc(4);
    ptr[6] = myAlloc(5);
    ptr[7] = myAlloc(9);
    ptr[8] = myAlloc(55);
   
    for (int i = 0; i < 9; i++) {
        assert(ptr[i] != NULL);
    }

    for (int i = 0; i < 9; i++) {
        assert(((int)ptr[i]) % 8 == 0);
    }
    dispMem();
    exit(0);
}
