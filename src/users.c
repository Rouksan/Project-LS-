/*
             @author : Loubna
             Date    : 01.10.2024
         
              users.c file
*/
#include "users.h"
#include <stdlib.h>
#include <string.h>

// Function to add a new user to users.csv
void addUser() {
    FILE *file = fopen("data/users.csv", "a");
    if (file == NULL) {
        printf("Unable to open users.csv\n");
        return;
    }

    int userID;
    char username[50], password[50], role[20];

    // Check for unique User ID
    int idExists;
    do {
        printf("Enter a unique User ID: ");
        scanf("%d", &userID);
        getchar(); // To consume the newline character

        // Check for uniqueness by reading the file
        FILE *checkFile = fopen("data/users.csv", "r");
        if (checkFile == NULL) {
            printf("Unable to open users.csv for checking ID uniqueness.\n");
            fclose(file);
            return;
        }

        idExists = 0; // Reset uniqueness flag
        char line[256];
        while (fgets(line, sizeof(line), checkFile)) {
            int currentID;
            sscanf(line, "%d,", &currentID);
            if (currentID == userID) {
                idExists = 1; // ID already exists
                printf("User ID %d already exists. Please enter a unique ID.\n", userID);
                break;
            }
        }
        fclose(checkFile); // Close the checking file
    } while (idExists);

    printf("Enter Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';
    
    printf("Enter Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    printf("Enter Role (admin/librarian/user): ");
    fgets(role, sizeof(role), stdin);
    role[strcspn(role, "\n")] = '\0';

    // Append the new user
    fprintf(file, "%d,%s,%s,%s\n", userID, username, password, role);
    fclose(file);
    printf("User added successfully.\n");
}


// Function to remove a user from users.csv and their transactions from transactions.csv
void removeUser() {
    FILE *usersFile = fopen("data/users.csv", "r");
    FILE *tempUsersFile = fopen("data/temp_users.csv", "w");
    FILE *transactionsFile = fopen("data/transactions.csv", "r");
    FILE *tempTransactionsFile = fopen("data/temp_transactions.csv", "w");

    if (usersFile == NULL || tempUsersFile == NULL || transactionsFile == NULL || tempTransactionsFile == NULL) {
        printf("Unable to open necessary files.\n");
        return;
    }

    int userID;
    printf("Enter User ID to remove: ");
    scanf("%d", &userID);

    char userLine[256];
    // Copy all users except the one to be removed
    while (fgets(userLine, sizeof(userLine), usersFile)) {
        int currentUserID;
        sscanf(userLine, "%d,", &currentUserID);
        if (currentUserID != userID) {
            fputs(userLine, tempUsersFile);  // Copy user data
        }
    }

    fclose(usersFile);
    fclose(tempUsersFile);

    // Now handle the removal of the user's transactions
    char transactionLine[256];
    while (fgets(transactionLine, sizeof(transactionLine), transactionsFile)) {
        int currentTransactionID, currentUserID;
        sscanf(transactionLine, "%d,%d,", &currentTransactionID, &currentUserID);
        if (currentUserID != userID) {
            fputs(transactionLine, tempTransactionsFile);  // Copy transaction data
        }
    }

    fclose(transactionsFile);
    fclose(tempTransactionsFile);

    // Replace original files with updated files
    remove("data/users.csv");
    rename("data/temp_users.csv", "data/users.csv");

    remove("data/transactions.csv");
    rename("data/temp_transactions.csv", "data/transactions.csv");

    printf("User and their transactions removed successfully.\n");
}
// Function to view all users from users.csv
void viewAllUsers() {
    FILE *file = fopen("data/users.csv", "r");
    if (file == NULL) {
        printf("Unable to open users.csv\n");
        return;
    }

    char line[256];
    printf("\n--- All Users ---\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}