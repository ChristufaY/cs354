#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void modify(int x[5][5], int m, int n) {
        for(int i = 0; i < m; i++) {
                for(int j = 0; j < n; j++) {
                        x[i][j] = 0;
                        printf("%d", x[i][j]);
                }
		printf("\n");
        }
	printf("\n");
}

void f(int num1, int num2, int *ptr3) {
	int temp = *(ptr3 + num1);
	ptr3[num1] = ptr3[num2];
	*(ptr3 + num2) = temp;
}

void update(int *x) {
    int *a;
a = malloc(sizeof(int));

a = x;

*x = 44;

    printf("%d ", *a);
}

void func(void) {
static int a = 10;
a++;
printf("%d ", a);
}

struct Data {
	int n;
	char* name;
	int* arr[3];
};

int main(void) {

//	int **x = malloc(sizeof(int *) * 5);
//	for(int i = 0; i < 5; i++) {
//		x[i] = malloc(sizeof(int)*5);
//	}
//	for(int i = 0; i < 5; i++) {
//      	for(int j = 0; j < 5; j++;
//            	x[i][j] = i*j;
//        	}
//	}
//	modify(x, 5, 5);
//	return 0;
//char destination[] = "spiderman";
//char source[] = "batman";
//strcpy(destination, source);
//printf("%s %s %d %d", destination, source, strlen(destination), sizeof(source));

//int i = 11;
//int *ptr1 = &i;
//int **ptr2 = &ptr1;
//int ***ptr3 = &ptr2;

//printf("%p, %p, %p, %p\n", ptr1, ptr2, ptr3, &ptr3);
//printf("%d, %p, %p, %p\n", *ptr1, *ptr2, *ptr3, *(&ptr3));
//0xfffbefac, 0xfffbefb0, 0xfffbefb4, 0xfffbefb8
//11, 0xfffbefac, 0xfffbefb0, 0xfffbefb4,

//0x_0b4 
//0x_0b8
//0x_0c0
//0x_0c8

//int *x = malloc(sizeof(int) * 5);
//for(int i = 0; i < 5; i++) {
//*(x + i) = i;
//}
//CODE A
// Assuming ints are 4 bytes and x = 1000
//int *y = x + 1;
//printf("%p %p %ld\n", x, y, y - x);
//Output: 1000, 1001, 1
//int *y = x + 4;
//printf("%p %p %ld"\n, x, y, *(y - 3));
//Output: 1000, 1016, 1

//int a = 4;
//int b = 1;
//int c[] = {13, 3, 21, 8, 2, 5};
//f(a,b,c);
//printf("%i. %i, %i, %i, %i, %i\n", c[0], c[1], c[2], c[3], c[4], c[5]);

//int a = 11;
//int *b = &a;
//printf("%d ", *b);
//update(b);
//printf("%d ", *b);
//int n = 0;
//n = 9;
//int **magic_square;
//magic_square = malloc(sizeof(int *) * 3);
//for(int i = 0; i < 3; i++) 
//	*(magic_square + i) = malloc(sizeof(int) * 3);

//*(*(magic_square + 0) + 0) = 8;
//*(*(magic_square + 0) + 1) = 1;
//*(*(magic_square + 0) + 2) = 6;
//*(*(magic_square + 1) + 0) = 3;
//*(*(magic_square + 1) + 1) = 5;
//*(*(magic_square + 1) + 2) = 7;
//*(*(magic_square + 2) + 0) = 4;
//*(*(magic_square + 2) + 1) = 9;
//*(*(magic_square + 2) + 2) = 2;

//for(int i = 0; i < 3; i++) {
//                for(int j = 0; j < 3; j++) {
//                        printf("%d,", *(*(magic_square + i) + j));
//                }
//        printf("\n");
//}
//func();
//func();
//printf("\n");
struct Data *sptr = malloc(sizeof(struct Data));
sptr->n = 33;
sptr->name = malloc(12 * sizeof(char));
if (sptr->name == NULL) {
	printf("Unable to allocate memory.\n");
	exit(1);
}
for (int i = 0; i < 3; i++) {
	sptr->arr[i] = malloc(4 * sizeof(int));
	if (sptr->arr[i] == NULL) {
		printf("Unable to allocate memory.\n");
		exit(1);
	}
	for (int j = 0; j < 4; j++)
		*(sptr->arr[i] + j) = 0;
}
for(int i = 0; i < 3; i ++) {
	for(int j = 0; j < 4; j++){
		printf("%d",sptr->arr[i][j]);
	}
printf("\n");
}
return 0;


}


//void modify (int x[5][5], int m, int n)

//void modify(int *x[], int m, int n)
