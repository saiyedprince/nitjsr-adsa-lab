#include <stdio.h>
#include <stdlib.h>

#define N 100

int st1[N], st2[N];
int top1 = -1, top2 = -1;

// Stack push and pop for st1
void push1(int x) { st1[++top1] = x; }
int pop1() { return st1[top1--]; }

// Stack push and pop for st2
void push2(int x) { st2[++top2] = x; }
int pop2() { return st2[top2--]; }

// Make push operation costly
void enqueue(int x) {
    // Move all elements from st1 to st2
    while (top1 != -1) {
        push2(pop1());
    }
    // Push new element to st1
    push1(x);
    // Move back all elements to st1
    while (top2 != -1) {
        push1(pop2());
    }
}

// Dequeue operation costly
int dequeue() {
    if (top2 == -1) {
        if (top1 == -1) {
            printf("Queue is empty\n");
            return -1;
        }
        // Transfer all elements from st1 to st2
        while (top1 != -1) {
            st2[++top2] = pop1();
        }
    }
    return pop2();
}
int main() {
    enqueue(10);
    enqueue(20);
    enqueue(30);
    printf("Dequeued element: %d\n", dequeue()); // 10
    enqueue(40);
    printf("Dequeued element: %d\n", dequeue()); // 20
    printf("Dequeued element: %d\n", dequeue()); // 30
    printf("Dequeued element: %d\n", dequeue()); // 40
    printf("Dequeued element: %d\n", dequeue()); // Queue empty

    return 0;
}
