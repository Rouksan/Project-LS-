/*
             @author : Rouksan
             Date    : 09.10.2024
         
             beep_menu.c file
*/
#include "menu.h"
#include "books.h"
#include "users.h"
#include "transactions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Helper function for authentication
int authenticate(char *role) {
    FILE *file = fopen("data/users.csv", "r");
    if (file == NULL) {
        printf("Unable to open users.csv\n");
        return 0;
    }

    char inputUser[50], inputPass[50];
    printf("Enter Username: ");
    scanf("%s", inputUser);
    printf("Enter Password: ");
    scanf("%s", inputPass);

    char line[256];
    int authenticated = 0;
    while (fgets(line, sizeof(line), file)) {
        int userID;
        char username[50], password[50], userRole[20];
        sscanf(line, "%d,%[^,],%[^,],%s", &userID, username, password, userRole);
        if (strcmp(username, inputUser) == 0 && strcmp(password, inputPass) == 0 && strcmp(userRole, role) == 0) {
            authenticated = 1;
            break;
        }
    }

    fclose(file);
    if (!authenticated) {
        printf("Authentication failed.\n");
    }
    return authenticated;
}

void showMainMenu() {
    int choice;
    Beep(750, 800);
    printf("\n--- School Library System ---\n");
    printf("1. Admin\n");
    printf("2. Librarian\n");
    printf("3. User\n");
    printf("4. Exit\n");
    printf("Select Role: ");
    scanf("%d", &choice);
    switch(choice) {
        case 1:
            if (authenticate("admin")) {
                showAdminMenu();
            }
            break;
        case 2:
            if (authenticate("librarian")) {
                showLibrarianMenu();
            }
            break;
        case 3:
            if (authenticate("user")) {
                showUserMenu();
            }
            break;
        case 4:
            printf("Exiting the system. Goodbye!\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
    }
}

void showAdminMenu() {
    int choice;
    do {
        Beep(750, 800);
        printf("\n--- Admin Menu ---\n");
        printf("1. Add Book\n");
        printf("2. Remove Book\n");
        printf("3. View All Books\n");
        printf("4. Add User\n");
        printf("5. Remove User\n");
        printf("6. View All Users\n");
        printf("7. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                addBook();
                break;
            case 2:
                removeBook();
                break;
            case 3:
                viewAllBooks();
                break;
            case 4:
                addUser();
                break;
            case 5:
                removeUser();
                break;
            case 6:
                viewAllUsers();
                break;
            case 7:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while(choice !=7);
}

void showLibrarianMenu() {
    int choice;
    do {
        Beep(750, 800);
        printf("\n--- Librarian Menu ---\n");
        printf("1. Issue Book\n");
        printf("2. Return Book\n");
        printf("3. View All Transactions\n");
        printf("4. Search Books\n");
        printf("5. View All Books\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                issueBook();
                break;
            case 2:
                returnBook();
                break;
            case 3:
                viewAllTransactions();
                break;
            case 4:
                searchBooks();
                break;
            case 5:
                viewAllBooks();
                break;
            case 6:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while(choice !=6);
}

void showUserMenu() {
    int choice;
    char username[50]; // Assume we store the username after authentication
    printf("Enter your username: ");
    scanf("%s", username);
    
    do {
        Beep(750, 800);
        printf("\n--- User Menu ---\n");
        printf("1. Search Books\n");
        printf("2. View Available Books\n");
        printf("3. View My Transactions\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                searchBooks();
                break;
            case 2:
                viewAvailableBooks(); // Shows only available books
                break;
            case 3:
                viewUserTransactions(username); // Shows only user's transactions
                break;
            case 4:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while(choice !=4);
}
