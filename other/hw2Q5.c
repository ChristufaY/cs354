#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	char title[55];
	char *author;
	int pages;
} Book;

typedef struct {
	int numBooks;
	Book *books[22];
} Shelf;

int main(void) {
	Shelf bookShelf[11];
	Book book;
	Book book2;
	bookShelf[3].books[7] = &book;
	bookShelf[3].books[8] = &book2;
	//statement added here
	//bookShelf[3].books[7] = malloc(sizeof(Book)); this one got seg fault

	//bookShelf[3].books[7]->author = malloc(sizeof(char)); workds
	bookShelf[3].books[7]->author = malloc(sizeof(char));
	strcpy(bookShelf[3].books[7]->author, "Seuss");
	bookShelf[3].books[8]->author = malloc(sizeof(char)* 50);
	printf("Author: %s\n", bookShelf[3].books[7]->author);

return 0;

}
