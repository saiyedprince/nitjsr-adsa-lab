#include <stdio.h>
#include <stdlib.h>

#define MAX 100
typedef struct Node {
    int data;
    struct Node* next;
} Node; 
void insertSorted(Node** bucket, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;

    if (*bucket == NULL || value < (*bucket)->data) {
        newNode->next = *bucket;
        *bucket = newNode;
    } else {
        Node* current = *bucket;
        while (current->next != NULL && current->next->data < value)
            current = current->next;
        newNode->next = current->next;
        current->next = newNode;
    }
}
void freeBuckets(Node* buckets[], int size) {
    for (int i = 0; i < size; i++) {
        Node* current = buckets[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
}

int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d integers:\n", n);
    int max = 0;
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
        if (arr[i] > max)
            max = arr[i];
    }
    Node* buckets[MAX] = {NULL};
    for (int i = 0; i < n; i++) {
        int index = (arr[i] * (MAX - 1)) / max; 
        insertSorted(&buckets[index], arr[i]);
    }
    int k = 0;
    for (int i = 0; i < MAX; i++) {
        Node* current = buckets[i];
        while (current != NULL) {
            arr[k++] = current->data;
            current = current->next;
        }
    }
    printf("Sorted array:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
    freeBuckets(buckets, MAX);

    return 0;
}