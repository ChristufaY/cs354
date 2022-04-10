#include <stdlib.h>
#include <stdio.h>

void main(int argc, char **argv) {
    int *int_arr;
    long *long_arr;
    int_arr = (int*)malloc(sizeof(int) * 3);
    long_arr = (long*)malloc(sizeof(long) * 3);
    *(int_arr) = 0;
    *(int_arr+ 1) = 1;
    *(int_arr+ 2) = 2;

 

    *(long_arr + 0) = 10;
    *(long_arr + 1) = 11;
    *(long_arr + 2) = 12;

 

    printf("%lu\n", sizeof(int));
    printf("%lu\n", sizeof(long));
    printf("%lu\n", sizeof(double));
    printf("%lu\n", sizeof(char));
    printf("%lu\n", sizeof(short));

 

    int i = 0;
    for (i = 0 ; i < 3; i++) {
        printf("%d, %p, %d\n", *(int_arr + i), (int_arr + i), int_arr[i]);
    }

 

//    for (i = 0 ; i < 3; i++) {
//        printf("%ld, %p, %ld\n", *(long_arr + i), (long_arr + i), long_arr[i]);
//    }


}
