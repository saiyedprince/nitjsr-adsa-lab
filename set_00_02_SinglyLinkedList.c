#include <stdio.h>
#include <stdlib.h>

// node structure
struct Node {
    int data;
    struct Node* next;
};

struct Node* head = NULL;

// inserting a node at the end
void insert(int val) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = val;
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    } else {
        struct Node* temp = head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }
    printf("Inserted %d\n", val);
}

// deleteing a node by value
void delete(int val) {
    struct Node *temp = head, *prev = NULL;

    while (temp != NULL && temp->data != val) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Value %d not found\n", val);
        return;
    }

    if (prev == NULL) {
        head = temp->next;
    } else {
        prev->next = temp->next;
    }

    free(temp);
    printf("Deleted %d\n", val);
}

// searching by value
void search(int val) {
    struct Node* temp = head;
    int pos = 1;

    while (temp != NULL) {
        if (temp->data == val) {
            printf("Value %d found at position %d\n", val, pos);
            return;
        }
        temp = temp->next;
        pos++;
    }

    printf("Value %d not found\n", val);
}

// traversel and display the list
void traverse() {
    struct Node* temp = head;
    if (temp == NULL) {
        printf("List is empty\n");
        return;
    }

    printf("Linked List: ");
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

// Main menu-driven operations
int main() {
    int choice, value;

    while (1) {
        printf("\nOperations menu\n");
        printf("1. Insert\n");
        printf("2. Delete\n");
        printf("3. Search\n");
        printf("4. Traverse\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter a value to insert: ");
                scanf("%d", &value);
                insert(value);
                break;
            case 2:
                printf("Enter a value to delete: ");
                scanf("%d", &value);
                delete(value);
                break;
            case 3:
                printf("Enter a value to search: ");
                scanf("%d", &value);
                search(value);
                break;
            case 4:
                traverse();
                break;
            case 5:
                printf("programe exited \n");
                exit(0);
            default:
                printf("Invalid choice \n");
        }
    }

    return 0;
}
