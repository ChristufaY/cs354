#include <stdio.h>
#include <string.h>

typedef union {
    double d;
    int *i;
}U;

int main() {
    U temp;
    int x = 44;
    temp.d = 44.4;
    temp.i = &x;
    printf("%zu, %f\n", sizeof(temp), temp.d);
    return 0;
}
