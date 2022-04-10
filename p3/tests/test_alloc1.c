// a simple 8 byte allocation
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"

int main() {
    assert(myInit(4096) == 0);
    dispMem();
    void* ptr = myAlloc(8);
    dispMem();
    assert(ptr != NULL);
    exit(0);
}
