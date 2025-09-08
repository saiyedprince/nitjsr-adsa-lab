#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

typedef struct {
    int data[SIZE];
    int front, rear;
} Queue;

// Initialize queue
void initQueue(Queue *q) {
    q->front = q->rear = -1;
}

// Check if queue is empty
int isEmpty(Queue *q) {
    return q->front == -1;
}

// Enqueue operation for queue
void enqueue(Queue *q, int x) {
    if ((q->rear + 1) % SIZE == q->front) {
        printf("Queue overflow\n");
        return;
    }
    if (q->front == -1) q->front = 0;
    q->rear = (q->rear + 1) % SIZE;
    q->data[q->rear] = x;
}

// Dequeue operation for queue
int dequeue(Queue *q) {
    if (isEmpty(q)) {
        printf("Queue underflow\n");
        return -1;
    }
    int x = q->data[q->front];
    if (q->front == q->rear) {
        q->front = q->rear = -1; // queue empty
    } else {
        q->front = (q->front + 1) % SIZE;
    }
    return x;
}

// Stack using two queues
typedef struct {
    Queue q1, q2;
} Stack;

// Initialize stack
void initStack(Stack *s) {
    initQueue(&s->q1);
    initQueue(&s->q2);
}

// Push operation (costly enqueue)
void push(Stack *s, int x) {
    // Enqueue x to q2
    enqueue(&s->q2, x);

    // Move all elements from q1 to q2
    while (!isEmpty(&s->q1)) {
        enqueue(&s->q2, dequeue(&s->q1));
    }

    // Swap q1 and q2
    Queue temp = s->q1;
    s->q1 = s->q2;
    s->q2 = temp;
}
// pop operation costly
int pop(Stack *s) {
    if (isEmpty(&s->q1)) {
        printf("Stack underflow\n");
        return -1;
    }

    // Move elements from q1 to q2 except the last one
    while ((s->q1.front != s->q1.rear) && !isEmpty(&s->q1)) {
        enqueue(&s->q2, dequeue(&s->q1));
    }

    // The last dequeued element from q1 is top of stack
    int popped = dequeue(&s->q1);

    // Swap q1 and q2
    Queue temp = s->q1;
    s->q1 = s->q2;
    s->q2 = temp;

    return popped;
}

// Main function to demonstrate
int main() {
    Stack s;
    initStack(&s);

    push(&s, 10);
    push(&s, 20);
    push(&s, 30);

    printf("Popped: %d\n", pop(&s)); // 30
    printf("Popped: %d\n", pop(&s)); // 20

    push(&s, 40);

    printf("Popped: %d\n", pop(&s)); // 40
    printf("Popped: %d\n", pop(&s)); // 10
    printf("Popped: %d\n", pop(&s)); // Stack underflow

    return 0;
}
