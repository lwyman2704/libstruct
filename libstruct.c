//********************************************************
// libstruct.c
// Author: Lorenzo Wyman
// UCFID: 5464079
// Date: 11/16/2024
// Class: COP 3223, Professor Parra
// Purpose: This program allocates memory for books in a library and stores, removes, checks out
// returns or finds the books based off what is read from the input file.
// Input: The files that are given
//
// Output: (to the command line) The outputs from the functions
// 
//********************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libstruct.h"

//***********************************************************************
// int bookParser (char *line, char *bookTitle, char *bookAuthor, 
//                 int *bookYear)
//
// Purpose: Parses the book, searches for commas and turns year 
//          into int value again so that rest of program can actually
//          read the file input.
// Output:  Prints debugging process to see where the function is currently at.
//          Helped out ALOTTT so i could see where it was incorrectly reading the file
// Precondition: Variables
//********************************************************
int bookParser(char *line, char *bookTitle, char *bookAuthor, int *bookYear) {

    printf("Beginning to parse line: %s", line);

    char *finalComma = strrchr(line, ',');

    if (finalComma == NULL) { 
        printf("Final comma is not found :(\n");
        return 0; 
    }

    *bookYear = atoi(finalComma + 1); //atoi was said to turn that bookYear into a integer value
    *finalComma = '\0'; // uses the null terminator before the year part
    char *firstComma = strchr(line, ','); //these lines of code find the comma that will seperate the author name from the title

    if (firstComma == NULL) {
        printf("First comma is not found :(\n");
        return 0; //invalid format (if it does not find the comma)
    }

    *firstComma = '\0'; 

    strcpy(bookTitle, line); //copies title
    strcpy(bookAuthor, firstComma + 1); //copies author name

    char *authorSpaceTrim = bookAuthor;

    while (*authorSpaceTrim == ' '){ //this removes a extra space that got added between the author name and title that got annoying to deal with so had to find solution
        authorSpaceTrim++;
    }

    if (authorSpaceTrim != bookAuthor){
        strcpy(bookAuthor, authorSpaceTrim);
    }

    printf("Successfully parsed: '%s, author %s, with year %d'\n", bookTitle, bookAuthor, bookYear);
    return 1; 
}

//********************************************************
// void actionHandler (Library *library, char *action, char *line)
//
// Purpose: Correctly reads the file, and when it sees the "action"
// above the line of code in the file it calls upon that function.
// Output:  Debugging outputs.
//********************************************************
void actionHandler(Library *library, char *action, char *line) {

    printf("\n");
    printf("Current Action: '%s'\n", action);
    printf("Current Line: %s", line);

    char bookTitle[256];
    char bookAuthor[256];
    int bookYear;

    if (!bookParser(line, bookTitle, bookAuthor, &bookYear)) {
        printf("file parsing failed :( for line: %s\n", line);
        return;
    }

        if (strcmp(action, "add") == 0) { //this section reads the lines above the titles and such that are expected to be in the test files

            addBook(library, bookTitle, bookAuthor, bookYear);

        } else if (strcmp(action, "remove") == 0) {

            findBook(library, bookTitle, bookAuthor, bookYear);
            removeBook(library, bookTitle, bookAuthor, bookYear);
            //checkoutBook(library, bookTitle, bookAuthor, bookYear);
            //returnBook(library, bookTitle, bookAuthor, bookYear);

        } else if (strcmp(action, "checkout") == 0){

           checkoutBook(library, bookTitle, bookAuthor, bookYear);

        } else if (strcmp(action, "return") == 0){

            returnBook(library, bookTitle, bookAuthor, bookYear);
        }
}

//********************************************************
// void structInitializer (char *fileName, Library *library)
//
// Purpose:  initializes the structs in the .h file, same with variables,
//           opens the file and does debugging
// Output:   Debugging messages.
//********************************************************
void structInitializer(char *fileName, Library *library) {

    //Library library;
    library -> count = 0;
    library -> library = malloc(sizeof(Book) * MAX); //make dynamic memory for the book struct

    if (library -> library == NULL){
        printf("mem allocation failed\n");
        return;
    }

    FILE *reader = fopen(fileName, "r");
    if (reader == NULL){

        printf("Failed to read file.\n");
        free(library -> library);
        return;

    } else {
        printf("\nSuccessfully opened file: %s", fileName);
        printf("\n");
    }

    char line[1024]; 
    char action[30];

    while (fgets(line, sizeof(line), reader)) {

        line[strcspn(line, "\n")] = '\0'; //removes that new line character

       
        if (sscanf(line, "%24s", action) != 1) { //reads and searches for the "add", "remove" for the proper operation

            continue; //igonres all lines that arent valid
        }

        if (strcmp(line, "print") == 0){ //skip that sneaky print line that had my program read the lines wrong

            continue;
        }

        if (fgets(line, sizeof(line), reader) == NULL) { //once theres nothing else for it to read in the file

            break; 
        }

        actionHandler(library, action, line); //calls on function that handles the sorting and stuff for books
    }

    fclose(reader);
    //free(library -> library);
}

//********************************************************
// void addBook(Library *library, char *bookTitle, char *bookAuthor, 
//              int bookYear)
//
// Purpose: Adds book to library, marks it as available (1), does error handling
// with edge cases, uses string functions to copy specific # of characters and then
// print that the book was added
// Output:  The book that was added and the number of books now in the library.
//********************************************************

void addBook(Library *library, char *bookTitle, char *bookAuthor, int bookYear) {

    if (library -> count == MAX) {
        printf("The library is full. Cannot add more books.\n");
        return;
    }

    for (int i = 0; i < library -> count; i++) { //if the book title, author and year match one alr stored it will say its alr in the library

        if (strcmp(library -> library[i].title, bookTitle) == 0 && strcmp(library -> library[i].author, bookAuthor) == 0 && library -> library[i].year == bookYear) {

            printf("Book with title %s, author %s, and year %d is already in the library.\n", bookTitle, bookAuthor, bookYear);
            printf("There are still %d books in the library.\n", library -> count);

            return;
        }
    }

    Book *tempBook = &library -> library[library -> count]; //points tempbook pointer to the first spot in the array

    strncpy(tempBook -> title, bookTitle, sizeof(tempBook -> title) - 1); //copys certain amount of letters then points like before
    tempBook -> title[sizeof(tempBook -> title) - 1] = '\0'; //terminates the string right after the amount of letters has been reached

    strncpy(tempBook -> author, bookAuthor, sizeof(tempBook -> author) - 1);
    tempBook -> author[sizeof(tempBook -> author) - 1] = '\0';

    tempBook -> year = bookYear;
    tempBook -> isAvailable = 1; // changes from 0 to 1 to show its available
    library -> count++; //adds to the number of books in the library like in the pdf

    printf("Book with title %s, author %s, and year %d added to the library\n", bookTitle, bookAuthor, bookYear);
    
        if (library -> count == 1){
            printf("There is now %d book in the library.\n", library -> count);
        } else {
            printf("There are now %d books in the library.\n", library -> count);
        }
}

//********************************************************
// void removeBook(Library *library, char *bookTitle, char *bookAuthor,
//                 int bookYear) 
//
// Purpose: Same thing as add function but it removes the book, marks it 
// unavailable (0) and then subtracts from the count in the library
// Output: The book that was removed along with how many books in the library
//          now.
//********************************************************
void removeBook(Library *library, char *bookTitle, char *bookAuthor, int bookYear) {

    for (int i = 0; i < library -> count; i++) { //just like in add book, only this time theres '--' infront of library -> count that removes the book and updates the value

        if (strcmp(library -> library[i].title, bookTitle) == 0 && strcmp(library -> library[i].author, bookAuthor) == 0 && library -> library[i].year == bookYear) {

            for (int j = i; j < library -> count - 1; j++){
                library -> library[j] = library -> library[j + 1];
            }
            library -> count--; // replace with thhe last book
            printf("Book with title %s, author %s, and year %d has been removed from the library\n", bookTitle, bookAuthor, bookYear);
            printf("There are now %d books in the library.\n", library -> count);
            return;
        }
    }
    
    printf("Book not found\n"); //if the book is not found
}

//********************************************************
// void printLibrary(Library *library)
//
// Purpose: Prints the books currently in the library
// Output:  The books currently in the library

//********************************************************
void printLibrary(Library *library){
    // just prints the title, author, year and availablity of all books
    // print out if books are available/checked out
    //  if the book available
    
    printf("\n");
    printf("Books currently in library (%d):\n", library -> count);

    for (int i = 0; i < library -> count; i++) {

        Book *book = &library -> library[i]; //points to the current book

        if (book -> isAvailable){

            printf("Book with title %s, author %s, and year %d is in the library\n", book -> title, book -> author, book -> year);
        }
        
    }
}

//********************************************************
// Book* findBook(Library *library, char *bookTitle, char *bookAuthor, 
//                int bookYear)
//
// Purpose:  Finds the book that is supposed to be removed per the pdf instructions
// and return a pointer to that active library struct
// Output: The book that is found with matching year, author and title.
//********************************************************
Book* findBook(Library *library, char *bookTitle, char *bookAuthor, int bookYear){
// search for book by title, return pointer to matchin Book structif found
//output statement if book found
//if book not found return null, if found return the pointer to struct found

    for (int i = 0; i < library -> count; i++) { //if the book title, author and year match one alr stored it will say its alr in the library

        if (strcmp(library -> library[i].title, bookTitle) == 0 && strcmp(library -> library[i].author, bookAuthor) == 0 && library -> library[i].year == bookYear) {

            printf("Book with title %s, author %s, and year %d is found!\n", bookTitle, bookAuthor, bookYear);

            return &library -> library[i];
        }
    }
    return NULL;
}

//********************************************************
// void checkoutBook(Library *library, char *bookTitle, char *bookAuthor,
//                   int bookYear)
//
// Purpose: Checks out a book, checks to see if it is alr checked out and if so
// prints necessary statement. changes it back to 0 for not available.
// Output:          Prints the game output to the terminal.
// Precondition:        round >= 1.
// Postcondition:   None.
//********************************************************
void checkoutBook(Library *library, char *bookTitle, char *bookAuthor, int bookYear){
// output statement if checked out
// if alr checked out print statement
// print error message if not found
// changes isavailable to 0 if checked out

    for (int i = 0; i < library -> count; i++) { //if the book title, author and year match one alr stored it will say its alr in the library

        Book *book = &library -> library[i];

        if (strcmp(book -> title, bookTitle) == 0 && strcmp(book -> author, bookAuthor) == 0 && book -> year == bookYear){   

            if (book -> isAvailable){

                book -> isAvailable = 0; //marks the book that is pointed to as checked out
                printf("Book with title %s, author %s, and year %d is checked out\n", bookTitle, bookAuthor, bookYear);

            } else {

                printf("Book with title %s, author %s, and year %d is already checked out\n", bookTitle, bookAuthor, bookYear);
            
            }
            return; //NEEDS TO BE OUTSIDE OF LOOP AKA IT WAS ON THE SAME LINE AS THAT BRACKET AND HAD ME TWEAKING FOR THE LONGEST
        }
    
    }
        printf("Book with title %s , author %s, and year %d not found\n", bookTitle, bookAuthor, bookYear);

}     

//********************************************************
// void returnBook(Library *library, char *bookTitle, char *bookAuthor, 
//                 int bookYear)
//
// Purpose:  Initiates return of the book per the input file, marks the book
// as a 1 for availability
// Output:  The book is either returned or not checked out. If neither it prints
// that the book is not even found in the library
//********************************************************
void returnBook(Library *library, char *bookTitle, char *bookAuthor, int bookYear){
//print statement
//print statement if not chekced out
//print error
//change isavailable to 1 for available again

    for (int i = 0; i < library -> count; i++) { //if the book title, author and year match one alr stored it will say its alr in the library

        Book *book = &library -> library[i];

        if (strcmp(book -> title, bookTitle) == 0 && strcmp(book -> author, bookAuthor) == 0 && book -> year == bookYear){ 

            if (!book -> isAvailable){
                book -> isAvailable = 1;
                printf("Book with title %s, author %s, and year %d is returned", bookTitle, bookAuthor, bookYear);
                printf("\n");

            } else if (book -> isAvailable != 0){ //0 checked out, 1 not checked out

                printf("Book with title %s, author %s, and year %d is not checked out\n", bookTitle, bookAuthor, bookYear);
            }
            return;
        }   
    }

    printf("Book not found\n");
}

//********************************************************
// int main(int argc, char **argv)
//
// Purpose:  Main function. As the name suggests.
// Output:  Nothing but function calls
// Precondition:   Functions are declared and arguments aswell
// Postcondition:   None.
//********************************************************
int main(int argc, char **argv){

Library library;

structInitializer(argv[1], &library);
printLibrary(&library);
free(library.library);

    return 0;
}