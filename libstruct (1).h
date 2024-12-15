#ifndef LIBSTRUCT_H //these are header guards to prevent the code from reusing the structs or including them multiple times
#define LIBSTRUCT_H

#define MAX 100 //only 100 books structures

typedef struct Book { //just a regular struct named boook

    char title[1024];
    char author[1024];
    int year;
    int isAvailable;

} Book;

typedef struct Library {

    int count;
    Book *library; //this is used so that i can create that dynamic array of books asked for 

} Library;

int bookParser(char *line, char *bookTitle, char *bookAuthor, int *bookYear); //helper functions
void actionHandler(Library *library, char *action, char *line);
void structInitializer(char *fileName, Library *library);

void addBook(Library *library, char *bookTitle, char *bookAuthor, int bookYear); //these are all prototype functions
void removeBook(Library *library, char *bookTitle, char *bookAuthor, int bookYear);
void printLibrary(Library *library);
Book* findBook(Library *library, char *bookTitle, char *bookAuthor, int bookYear);
void checkoutBook(Library *library, char *bookTitle, char *bookAuthor, int bookYear);
void returnBook(Library *library, char *bookTitle, char *bookAuthor, int bookYear);

#endif //just ends that header guard