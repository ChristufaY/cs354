C_FILES := $(wildcard *.c)
TARGETS := ${C_FILES:.c=}

all: ${TARGETS}

%: %.c
	gcc -I.. -g -m32 -Xlinker -rpath=.. -o $@ $< -L.. -lheap -std=gnu99

partA:
	./test_alloc1
	./test_alloc1_nospace
	./test_writeable
	./test_align1
	./test_alloc2
	./test_alloc2_nospace
	./test_align2
	./test_alloc3
	./test_align3

partB:
	./test_free1
	./test_free2
	./test_fit1
	./test_fit1_nospace
	./test_fit2
	./test_fit2_nospace

partC:
	./test_coalesce1
	./test_coalesce2
	./test_coalesce3

clean:
	rm -rf ${TARGETS} *.o

