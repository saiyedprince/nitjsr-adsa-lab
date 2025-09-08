#include <stdio.h>
#include <stdlib.h>

#define SIZE 5

// ---------------- ARRAY IMPLEMENTATION ----------------
typedef struct {
    int items[SIZE];
    int front, rear;
} CircularQueueArray;

void initArrayQueue(CircularQueueArray *q) {
    q->front = q->rear = -1;
}

int isArrayFull(CircularQueueArray *q) {
    return (q->rear + 1) % SIZE == q->front;
}

int isArrayEmpty(CircularQueueArray *q) {
    return q->front == -1;
}

void arrayEnqueue(CircularQueueArray *q, int value) {
    if (isArrayFull(q)) {
        printf("Array Queue is full\n");
        return;
    }
    if (isArrayEmpty(q)) q->front = 0;
    q->rear = (q->rear + 1) % SIZE;
    q->items[q->rear] = value;
}

int arrayDequeue(CircularQueueArray *q) {
    if (isArrayEmpty(q)) {
        printf("Array Queue is empty\n");
        return -1;
    }
    int value = q->items[q->front];
    if (q->front == q->rear)
        q->front = q->rear = -1;
    else
        q->front = (q->front + 1) % SIZE;
    return value;
}

void arrayDisplay(CircularQueueArray *q) {
    if (isArrayEmpty(q)) {
        printf("Array Queue is empty\n");
        return;
    }
    int i = q->front;
    while (1) {
        printf("%d ", q->items[i]);
        if (i == q->rear) break;
        i = (i + 1) % SIZE;
    }
    printf("\n");
}

// ---------------- LINKED LIST IMPLEMENTATION ----------------
typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node *front;
    Node *rear;
} CircularQueueList;

void initListQueue(CircularQueueList *q) {
    q->front = q->rear = NULL;
}

int isListEmpty(CircularQueueList *q) {
    return q->front == NULL;
}

void listEnqueue(CircularQueueList *q, int value) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;

    if (isListEmpty(q)) {
        q->front = q->rear = newNode;
        newNode->next = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
        q->rear->next = q->front;
    }
}

int listDequeue(CircularQueueList *q) {
    if (isListEmpty(q)) {
        printf("Linked List Queue is empty\n");
        return -1;
    }
    int value;
    if (q->front == q->rear) {
        value = q->front->data;
        free(q->front);
        q->front = q->rear = NULL;
    } else {
        Node *temp = q->front;
        value = temp->data;
        q->front = q->front->next;
        q->rear->next = q->front;
        free(temp);
    }
    return value;
}

void listDisplay(CircularQueueList *q) {
    if (isListEmpty(q)) {
        printf("Linked List Queue is empty\n");
        return;
    }
    Node *temp = q->front;
    do {
        printf("%d ", temp->data);
        temp = temp->next;
    } while (temp != q->front);
    printf("\n");
}

// ---------------- MAIN MENU ----------------
int main() {
    CircularQueueArray arrayQueue;
    CircularQueueList listQueue;
    int choice, method, value, n;

    initArrayQueue(&arrayQueue);
    initListQueue(&listQueue);

    printf("Choose Queue Implementation:\n");
    printf("1. Array\n");
    printf("2. Linked List\n");
    scanf("%d", &method);

    printf("Enter number of elements to enqueue: ");
    scanf("%d", &n);

    printf("Enter %d elements:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &value);
        if (method == 1)
            arrayEnqueue(&arrayQueue, value);
        else
            listEnqueue(&listQueue, value);
    }

    do {
        printf("\nMenu:\n");
        printf("1. Enqueue\n");
        printf("2. Dequeue\n");
        printf("3. Display\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter value to enqueue: ");
                scanf("%d", &value);
                if (method == 1)
                    arrayEnqueue(&arrayQueue, value);
                else
                    listEnqueue(&listQueue, value);
                break;
            case 2:
                if (method == 1)
                    printf("Dequeued: %d\n", arrayDequeue(&arrayQueue));
                else
                    printf("Dequeued: %d\n", listDequeue(&listQueue));
                break;
            case 3:
                if (method == 1)
                    arrayDisplay(&arrayQueue);
                else
                    listDisplay(&listQueue);
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 4);

    return 0;
}