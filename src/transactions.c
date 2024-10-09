/*
             @author : Kaushik
             Date    : 01.10.2024
         
              transactions.c file
*/ 

#include "transactions.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "menu.h"
// Function to issue book
void issueBook() {
    FILE *transactions = fopen("data/transactions.csv", "a");
    FILE *users = fopen("data/users.csv", "r");
    FILE *books = fopen("data/books.csv", "r+");
    
    if (transactions == NULL || users == NULL || books == NULL) {
        printf("Unable to open necessary files.\n");
        return;
    }

    int transactionID, userID, bookID;
    char issueDate[11], returnDate[11];

    // Check for unique Transaction ID
    int idExists;
    do {
        printf("Enter a unique Transaction ID: ");
        scanf("%d", &transactionID);
        getchar(); // To consume the newline character

        // Check for uniqueness by reading the transactions file
        FILE *checkFile = fopen("data/transactions.csv", "r");
        if (checkFile == NULL) {
            printf("Unable to open transactions.csv for checking ID uniqueness.\n");
            fclose(transactions);
            fclose(users);
            fclose(books);
            return;
        }

        idExists = 0; // Reset uniqueness flag
        char line[256];
        while (fgets(line, sizeof(line), checkFile)) {
            int currentID;
            sscanf(line, "%d,", &currentID);
            if (currentID == transactionID) {
                idExists = 1; // ID already exists
                printf("Transaction ID %d already exists. Please enter a unique ID.\n", transactionID);
                break;
            }
        }
        fclose(checkFile); // Close the checking file
    } while (idExists);

    // Get User ID
    printf("Enter User ID: ");
    scanf("%d", &userID);
    
    // Check if User ID exists
    int userExists = 0; // Flag to check if user exists
    char userLine[256];
    rewind(users);
    while (fgets(userLine, sizeof(userLine), users)) {
        int currentUserID;
        sscanf(userLine, "%d,", &currentUserID);
        if (currentUserID == userID) {
            userExists = 1; // User ID exists
            break;
        }
    }

    if (!userExists) {
        printf("User ID %d does not exist. Returning to the librarian menu.\n", userID);
        fclose(transactions);
        fclose(users);
        fclose(books);
        showLibrarianMenu();  // Return to the librarian menu
        return;
    }

    // Get Book ID
    printf("Enter Book ID: ");
    scanf("%d", &bookID);
    
    // Check book availability
    char lineBuffer[256];
    int bookAvailable = 0; // Flag to check availability
    rewind(books);
    while (fgets(lineBuffer, sizeof(lineBuffer), books)) {
        int currentID;
        char available[10];
        sscanf(lineBuffer, "%d,%*[^,],%*[^,],%*[^,],%s", &currentID, available); // Extracting bookID and availability
        if (currentID == bookID && strcmp(available, "Yes") == 0) {
            bookAvailable = 1; // Book is available
            break;
        }
    }

    // If book is not available, inform the user and return to the menu
    if (!bookAvailable) {
        printf("Book ID %d is not available for issuing. Returning to the librarian menu.\n", bookID);
        fclose(transactions);
        fclose(users);
        fclose(books);
        showLibrarianMenu(); // Return to the librarian menu
        return;
    }

    // Get Issue Date
    printf("Enter Issue Date (YYYY-MM-DD): ");
    getchar(); // Consume newline
    fgets(issueDate, sizeof(issueDate), stdin);
    issueDate[strcspn(issueDate, "\n")] = '\0'; // Remove newline character

    // Set Return Date as "-"
    strcpy(returnDate, "-");

    // Append the transaction
    fprintf(transactions, "%d,%d,%d,%s,%s\n", transactionID, userID, bookID, issueDate, returnDate);

    // Update book availability to 'No'
    FILE *tempBooks = fopen("data/temp_books.csv", "w");  // Create a temporary file for updated books data
    rewind(books);
    while (fgets(lineBuffer, sizeof(lineBuffer), books)) {
        int currentID;
        sscanf(lineBuffer, "%d,", &currentID);
        if (currentID == bookID) {
            char title[100], author[100], genre[50], available[10];
            sscanf(lineBuffer, "%d,%[^,],%[^,],%[^,],%s", &currentID, title, author, genre, available); // Extract book info
            fprintf(tempBooks, "%d,%s,%s,%s,No\n", currentID, title, author, genre); // Update availability to 'No'
        } else {
            fputs(lineBuffer, tempBooks);  // Write unchanged book records
        }
    }

    fclose(transactions);
    fclose(users);
    fclose(books);
    fclose(tempBooks);

    remove("data/books.csv"); // Remove the original file
    rename("data/temp_books.csv", "data/books.csv"); // Rename temp file to original

    printf("Book issued successfully. Returning to the librarian menu.\n");
    
    return;  // Return to the librarian menu
}


// Function to return a book
// Function to return a book and update the return date
void returnBook() {
    FILE *transactions = fopen("data/transactions.csv", "r");
    FILE *books = fopen("data/books.csv", "r");

    if (transactions == NULL || books == NULL) {
        printf("Unable to open necessary files.\n");
        return;
    }

    int transactionID, bookID;
    char returnDate[11];

    // Get Transaction ID
    printf("Enter Transaction ID for the book return: ");
    scanf("%d", &transactionID);

    // Read all transactions into memory
    char lineBuffer[256];
    char updatedTransactions[5000] = "";  // Buffer to hold the updated transactions
    int found = 0;  // Flag to check if the transaction is found

    // Iterate over transactions to find the correct one and update it
    while (fgets(lineBuffer, sizeof(lineBuffer), transactions)) {
        int currentID, userID, transBookID;
        char issueDate[11], existingReturnDate[11];
        sscanf(lineBuffer, "%d,%d,%d,%[^,],%s", &currentID, &userID, &transBookID, issueDate, existingReturnDate);

        if (currentID == transactionID && strcmp(existingReturnDate, "-") == 0) {
            found = 1;
            bookID = transBookID; // Capture book ID for later use

            // Get Return Date from the user
            printf("Enter Return Date (YYYY-MM-DD): ");
            getchar(); // Consume newline
            fgets(returnDate, sizeof(returnDate), stdin);
            returnDate[strcspn(returnDate, "\n")] = '\0'; // Remove newline character

            // Format the updated transaction record
            char updatedLine[256];
            sprintf(updatedLine, "%d,%d,%d,%s,%s\n", currentID, userID, transBookID, issueDate, returnDate);

            // Add the updated line to the buffer
            strcat(updatedTransactions, updatedLine);
        } else {
            // Add the original line to the buffer
            strcat(updatedTransactions, lineBuffer);
        }
    }

    fclose(transactions);

    if (!found) {
        printf("Transaction ID %d not found or the book has already been returned.\n", transactionID);
        fclose(books);
        showLibrarianMenu();  // Return to the librarian menu
        return;
    }

    // Rewrite the transactions file with updated data
    transactions = fopen("data/transactions.csv", "w");
    fputs(updatedTransactions, transactions);
    fclose(transactions);

    // Update the book availability in the books file
    FILE *tempBooks = fopen("data/temp_books.csv", "w");  // Create a temporary file for updated books data
    rewind(books);
    while (fgets(lineBuffer, sizeof(lineBuffer), books)) {
        int currentID;
        sscanf(lineBuffer, "%d,", &currentID);

        if (currentID == bookID) {
            char title[100], author[100], genre[50], available[10];
            sscanf(lineBuffer, "%d,%[^,],%[^,],%[^,],%s", &currentID, title, author, genre, available); // Extract book info
            fprintf(tempBooks, "%d,%s,%s,%s,Yes\n", currentID, title, author, genre); // Update availability to 'Yes'
        } else {
            fputs(lineBuffer, tempBooks);  // Write unchanged book records
        }
    }

    fclose(books);
    fclose(tempBooks);

    remove("data/books.csv");  // Remove the original file
    rename("data/temp_books.csv", "data/books.csv");  // Rename the temp file to original

    printf("Book returned successfully. Returning to the librarian menu.\n");

    return;  // Return to the librarian menu
}


// Function to view all transactions
void viewAllTransactions() {
    FILE *file = fopen("data/transactions.csv", "r");
    if (file == NULL) {
        printf("Unable to open transactions.csv\n");
        return;
    }

    char line[256];
    printf("\n--- All Transactions ---\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
fclose(file);
}
// Function to view transactions for a specific user
void viewUserTransactions() {
    FILE *file = fopen("data/transactions.csv", "r");
    if (file == NULL) {
        printf("Unable to open transactions.csv\n");
        return;
    }

    int userID;
    printf("Enter User ID to view transactions: ");
    scanf("%d", &userID);

    char line[256];
    int found = 0;
    printf("\n--- User Transactions ---\n");
    while (fgets(line, sizeof(line), file)) {
        int currentTransactionID, currentUserID, bookID;
        char issueDate[11], returnDate[11];
        sscanf(line, "%d,%d,%d,%[^,],%s", &currentTransactionID, &currentUserID, &bookID, issueDate, returnDate);
        if (currentUserID == userID) {
            printf("Transaction ID: %d, Book ID: %d, Issue Date: %s, Return Date: %s\n", currentTransactionID, bookID, issueDate, returnDate);
            found = 1;
        }
    }

    if (!found) {
        printf("No transactions found for User ID: %d\n", userID);
    }

    fclose(file);
}
