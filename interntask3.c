#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTACTS 100
#define NAME_LEN 50
#define PHONE_LEN 15
#define EMAIL_LEN 50

typedef struct {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
} Contact;

Contact contacts[MAX_CONTACTS];
int contact_count = 0;

void loadContacts();
void saveContacts();
void addContact();
void viewContacts();
void editContact();
void deleteContact();
void displayMenu();

int main() {
    loadContacts();

    int choice;
    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character

        switch (choice) {
            case 1:
                addContact();
                break;
            case 2:
                viewContacts();
                break;
            case 3:
                editContact();
                break;
            case 4:
                deleteContact();
                break;
            case 5:
                saveContacts();
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

void loadContacts() {
    FILE *file = fopen("contacts.txt", "r");
    if (file == NULL) {
        return;
    }

    while (fscanf(file, "%49s %14s %49s", contacts[contact_count].name, contacts[contact_count].phone, contacts[contact_count].email) != EOF) {
        contact_count++;
    }

    fclose(file);
}

void saveContacts() {
    FILE *file = fopen("contacts.txt", "w");
    if (file == NULL) {
        printf("Error saving contacts.\n");
        return;
    }

    for (int i = 0; i < contact_count; i++) {
        fprintf(file, "%s %s %s\n", contacts[i].name, contacts[i].phone, contacts[i].email);
    }

    fclose(file);
}

void addContact() {
    if (contact_count >= MAX_CONTACTS) {
        printf("Contact list is full.\n");
        return;
    }

    printf("Enter name: ");
    fgets(contacts[contact_count].name, NAME_LEN, stdin);
    contacts[contact_count].name[strcspn(contacts[contact_count].name, "\n")] = '\0'; // Remove newline character

    printf("Enter phone number: ");
    fgets(contacts[contact_count].phone, PHONE_LEN, stdin);
    contacts[contact_count].phone[strcspn(contacts[contact_count].phone, "\n")] = '\0'; // Remove newline character

    printf("Enter email address: ");
    fgets(contacts[contact_count].email, EMAIL_LEN, stdin);
    contacts[contact_count].email[strcspn(contacts[contact_count].email, "\n")] = '\0'; // Remove newline character

    contact_count++;
    printf("Contact added successfully.\n");
}

void viewContacts() {
    if (contact_count == 0) {
        printf("No contacts available.\n");
        return;
    }

    for (int i = 0; i < contact_count; i++) {
        printf("Contact %d:\n", i + 1);
        printf("Name: %s\n", contacts[i].name);
        printf("Phone: %s\n", contacts[i].phone);
        printf("Email: %s\n", contacts[i].email);
        printf("\n");
    }
}

void editContact() {
    int index;
    printf("Enter the contact number to edit (1 to %d): ", contact_count);
    scanf("%d", &index);
    getchar(); // Consume newline character

    if (index < 1 || index > contact_count) {
        printf("Invalid contact number.\n");
        return;
    }

    index--; // Convert to zero-based index

    printf("Enter new name (or press Enter to keep current): ");
    char input[NAME_LEN];
    fgets(input, NAME_LEN, stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        strcpy(contacts[index].name, input);
    }

    printf("Enter new phone number (or press Enter to keep current): ");
    fgets(input, PHONE_LEN, stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        strcpy(contacts[index].phone, input);
    }

    printf("Enter new email address (or press Enter to keep current): ");
    fgets(input, EMAIL_LEN, stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        strcpy(contacts[index].email, input);
    }

    printf("Contact updated successfully.\n");
}

void deleteContact() {
    int index;
    printf("Enter the contact number to delete (1 to %d): ", contact_count);
    scanf("%d", &index);
    getchar(); // Consume newline character

    if (index < 1 || index > contact_count) {
        printf("Invalid contact number.\n");
        return;
    }

    index--; // Convert to zero-based index

    for (int i = index; i < contact_count - 1; i++) {
        contacts[i] = contacts[i + 1];
    }

    contact_count--;
    printf("Contact deleted successfully.\n");
}

void displayMenu() {
    printf("\nContact Manager\n");
    printf("1. Add a new contact\n");
    printf("2. View contacts\n");
    printf("3. Edit a contact\n");
    printf("4. Delete a contact\n");
    printf("5. Exit\n");
}
