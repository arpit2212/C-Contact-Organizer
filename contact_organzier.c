#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Contact {
    char name[100];
    char phoneNumber[20];
    struct tm birthday;
};

// Define a custom comparison function for sorting contacts by name
int compareByName(const void *a, const void *b) {
    return strcmp(((struct Contact *)a)->name, ((struct Contact *)b)->name);
}

struct PhoneDirectory {
    struct Contact contacts[100];
    int contactCount;
};

void addContact(struct PhoneDirectory *directory, const char *name, const char *phoneNumber, const struct tm *birthday) {
    if (directory->contactCount < 100) {
        struct Contact newContact;
        strcpy(newContact.name, name);
        strcpy(newContact.phoneNumber, phoneNumber);
        newContact.birthday = *birthday;
        directory->contacts[directory->contactCount++] = newContact;
    } else {
        printf("Phone directory is full. Cannot add more contacts.\n");
    }
}

void displayDirectory(const struct PhoneDirectory *directory) {
    printf("Phone Directory (Sorted by Name):\n");
    qsort(directory->contacts, directory->contactCount, sizeof(struct Contact), compareByName);
    for (int i = 0; i < directory->contactCount; ++i) {
        struct Contact contact = directory->contacts[i];
        printf("Name: %s, Phone: %s, Birthday: %d/%d\n",
               contact.name, contact.phoneNumber,
               contact.birthday.tm_mon + 1, contact.birthday.tm_mday);
    }
}

void displayUpcomingBirthdays(const struct PhoneDirectory *directory) {
    time_t now = time(NULL);
    struct tm today = *localtime(&now);

    printf("Upcoming Birthdays:\n");
    for (int i = 0; i < directory->contactCount; ++i) {
        struct Contact contact = directory->contacts[i];
        if (contact.birthday.tm_mon == today.tm_mon && contact.birthday.tm_mday > today.tm_mday) {
            printf("Name: %s, Birthday: %d/%d\n",
                   contact.name, contact.birthday.tm_mon + 1, contact.birthday.tm_mday);
        }
    }
}

void searchContact(const struct PhoneDirectory *directory, const char *name) {
    int found = 0;
    for (int i = 0; i < directory->contactCount; ++i) {
        struct Contact contact = directory->contacts[i];
        if (strcmp(contact.name, name) == 0) {
            printf("Contact found:\n");
            printf("Name: %s, Phone: %s, Birthday: %d/%d\n",
                   contact.name, contact.phoneNumber,
                   contact.birthday.tm_mon + 1, contact.birthday.tm_mday);
            found = 1;
        }
    }
    if (!found) {
        printf("Contact with name '%s' not found.\n", name);
    }
}

void editContact(struct PhoneDirectory *directory, const char *name) {
    for (int i = 0; i < directory->contactCount; ++i) {
        struct Contact *contact = &directory->contacts[i];
        if (strcmp(contact->name, name) == 0) {
            printf("Editing contact: %s\n", contact->name);
            printf("Enter new phone number: ");
            fflush(stdin);
            fgets(contact->phoneNumber, sizeof(contact->phoneNumber), stdin);
            printf("Enter new birthday (MM/DD): ");
            char bdayStr[20];
            fgets(bdayStr, sizeof(bdayStr), stdin);
            sscanf(bdayStr, "%d/%d", &contact->birthday.tm_mon, &contact->birthday.tm_mday);
            contact->birthday.tm_mon--; // Adjust for 0-based month
            printf("Contact information updated.\n");
            return;
        }
    }
    printf("Contact with name '%s' not found.\n", name);
}

void deleteContact(struct PhoneDirectory *directory, const char *name) {
    for (int i = 0; i < directory->contactCount; ++i) {
        if (strcmp(directory->contacts[i].name, name) == 0) {
            for (int j = i; j < directory->contactCount - 1; ++j) {
                directory->contacts[j] = directory->contacts[j + 1];
            }
            directory->contactCount--;
            printf("Contact '%s' deleted.\n", name);
            return;
        }
    }
    printf("Contact '%s' not found.\n", name);
}

int main() {
    struct PhoneDirectory directory;
    directory.contactCount = 0;

    while (1) {
        printf("\nPhone Directory Menu:\n");
        printf("1. Add Contact\n");
        printf("2. Display Directory (Sorted by Name)\n");
        printf("3. Display Upcoming Birthdays\n");
        printf("4. Delete Contact\n");
        printf("5. Search for Contact\n");
        printf("6. Edit Contact\n");
        printf("7. Exit\n");

        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear the newline character from the input buffer.

        if (choice == 7) {
            break;
        }

        switch (choice) {
            case 1: {
                char name[100], phoneNumber[20];
                struct tm birthday;
                printf("Enter Name: ");
                fgets(name, sizeof(name), stdin);
                strtok(name, "\n"); // Remove the newline character.
                printf("Enter Phone Number: ");
                fgets(phoneNumber, sizeof(phoneNumber), stdin);
                strtok(phoneNumber, "\n"); // Remove the newline character.
                printf("Enter Birthday (MM/DD): ");
                char bdayStr[20];
                fgets(bdayStr, sizeof(bdayStr), stdin);
                sscanf(bdayStr, "%d/%d", &birthday.tm_mon, &birthday.tm_mday);
                birthday.tm_mon--; // Adjust for 0-based month

                addContact(&directory, name, phoneNumber, &birthday);
                printf("Contact added.\n");
                break;
            }
            case 2:
                displayDirectory(&directory);
                break;
            case 3:
                displayUpcomingBirthdays(&directory);
                break;
            case 4: {
                char nameToDelete[100];
                printf("Enter the name of the contact to delete: ");
                fgets(nameToDelete, sizeof(nameToDelete), stdin);
                strtok(nameToDelete, "\n"); // Remove the newline character.
                deleteContact(&directory, nameToDelete);
                break;
            }
            case 5: {
                char nameToSearch[100];
                printf("Enter the name of the contact to search for: ");
                fgets(nameToSearch, sizeof(nameToSearch), stdin);
                strtok(nameToSearch, "\n"); // Remove the newline character.
                searchContact(&directory, nameToSearch);
                break;
            }
            case 6: {
                char nameToEdit[100];
                printf("Enter the name of the contact to edit: ");
                fgets(nameToEdit, sizeof(nameToEdit), stdin);
                strtok(nameToEdit, "\n"); // Remove the newline character.
                editContact(&directory, nameToEdit);
                break;
            }
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    printf("Goodbye!\n");

    return 0;
}
