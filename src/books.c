/*
             @author : Rouksan
             Date    : 01.10.2024
         
              books.c file
*/ 
#include "books.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#define MAX_BOOKS 1000 // Adjust as needed

typedef struct {
    int id;
    char title[100];
    char author[100];
    char genre[50];
    char available[10];
} Book;

Book books[MAX_BOOKS];
int bookCount = 0;

// Function to load all books from the CSV file
void loadBooks() {
    FILE *file = fopen("data/books.csv", "r");
    if (file == NULL) {
        bookCount = 0; // If file doesn't exist, start with an empty list
        return;
    }

    char line[256];
    bookCount = 0;

    while (fgets(line, sizeof(line), file) && bookCount < MAX_BOOKS) {
        sscanf(line, "%d,%[^,],%[^,],%[^,],%s",
               &books[bookCount].id,
               books[bookCount].title,
               books[bookCount].author,
               books[bookCount].genre,
               books[bookCount].available);
        bookCount++;
    }

    fclose(file);
}

// Function to normalize availability input
void normalizeAvailability(char *available) {
    for (int i = 0; available[i]; i++) {
        available[i] = tolower((unsigned char)available[i]);
    }
}

// Function to add a new book to books.csv
void addBook() {
    loadBooks(); // Load existing books

    if (bookCount >= MAX_BOOKS) {
        printf("Error: Maximum number of books reached.\n");
        return;
    }

    int bookID;
    int idExists;

    // Prompt for Book ID
    do {
        printf("Enter a unique Book ID: ");
        scanf("%d", &bookID);
        getchar(); // Consume the newline character

        // Check for uniqueness
        idExists = 0; // Reset uniqueness flag
        for (int i = 0; i < bookCount; i++) {
            if (books[i].id == bookID) {
                idExists = 1; // ID already exists
                printf("Book ID %d already exists. Please enter a unique ID.\n", bookID);
                break;
            }
        }
    } while (idExists);

    char title[100], author[100], genre[50], available[10];

    printf("Enter Title: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0'; // Remove newline

    printf("Enter Author: ");
    fgets(author, sizeof(author), stdin);
    author[strcspn(author, "\n")] = '\0';

    printf("Enter Genre: ");
    fgets(genre, sizeof(genre), stdin);
    genre[strcspn(genre, "\n")] = '\0';

    int validAvailability = 0;
    while (!validAvailability) {
        printf("Is the book available? (Yes/No): ");
        fgets(available, sizeof(available), stdin);
        available[strcspn(available, "\n")] = '\0';

        normalizeAvailability(available);

        if (strcmp(available, "yes") == 0 || strcmp(available, "no") == 0) {
            available[0] = toupper(available[0]);
            validAvailability = 1;
        } else {
            printf("Invalid input. Please enter 'Yes' or 'No'.\n");
        }
    }

    // Open the file in append mode
    FILE *file = fopen("data/books.csv", "a");
    if (file == NULL) {
        perror("Error opening books.csv for appending");
        return;
    }

    // Append the new book
    fprintf(file, "%d,%s,%s,%s,%s\n", bookID, title, author, genre, available);
    fclose(file);

    printf("Book added successfully with ID: %d\n", bookID);

    // Update the in-memory array
    books[bookCount].id = bookID;
    strcpy(books[bookCount].title, title);
    strcpy(books[bookCount].author, author);
    strcpy(books[bookCount].genre, genre);
    strcpy(books[bookCount].available, available);
    bookCount++;
}

// Function to view all available books
void viewAvailableBooks() { 
    loadBooks(); // Load the books from the CSV file into the in-memory array

    int found = 0;
    printf("\n--- Available Books ---\n");

    for (int i = 0; i < bookCount; i++) {
        if (strcasecmp(books[i].available, "yes") == 0) {  // Case-insensitive check for availability
            printf("Book ID: %d\n", books[i].id);
            printf("Title: %s\n", books[i].title);
            printf("Author: %s\n", books[i].author);
            printf("Genre: %s\n", books[i].genre);
            printf("Available: %s\n\n", books[i].available);
            found = 1;
        }
    }

    if (!found) {
        printf("No books are currently available.\n");
    }
}

// Function to remove a book from books.csv
void removeBook() {
    FILE *file = fopen("data/books.csv", "r");
    FILE *temp = fopen("data/temp_books.csv", "w");
    if (file == NULL || temp == NULL) {
        printf("Unable to open books.csv\n");
        return;
    }

    int bookID;
    printf("Enter Book ID to remove: ");
    scanf("%d", &bookID);

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int currentID;
        sscanf(line, "%d,", &currentID);
        if (currentID != bookID) {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    // Replace original file with temp file
    remove("data/books.csv");
    rename("data/temp_books.csv", "data/books.csv");
    printf("Book removed successfully.\n");
}

// Function to view all books from books.csv
void viewAllBooks() {
    FILE *file = fopen("data/books.csv", "r");
    if (file == NULL) {
        printf("Unable to open books.csv\n");
        return;
    }

    char line[256];
    printf("\n--- All Books ---\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    printf("\n");
    fclose(file);
}

// Function to search books based on title, author, or genre
void searchBooks() {
    FILE *file = fopen("data/books.csv", "r");
    if (file == NULL) {
        printf("Unable to open books.csv\n");
        return;
    }

    char query[100];
    printf("Enter search query (Title/Author/Genre): ");
    getchar(); // To consume the newline character
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0';

    char line[256];
    int found = 0;
    printf("\n--- Search Results ---\n");
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, query)) {
            printf("%s", line);
            found = 1;
        }
    }

    if (!found) {
        printf("No matching books found.\n");
    }

    fclose(file);
}