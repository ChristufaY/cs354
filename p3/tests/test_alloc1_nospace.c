// allocation is too big to fit
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"

int main() {
    assert(myInit(4096)  == 0);
    assert(myAlloc(1)    != NULL);
    assert(myAlloc(4095) == NULL);
    dispMem();
    exit(0);
}
