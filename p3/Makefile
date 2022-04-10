myHeap: myHeap.c myHeap.h
	gcc -g -c -Wall -m32 -fpic myHeap.c
	gcc -shared -Wall -m32 -o libheap.so myHeap.o

clean:
	rm -rf myHeap.o libheap.so
