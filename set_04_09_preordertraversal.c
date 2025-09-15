#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};
#define MAX 100
struct Node* stack[MAX];
int top = -1;
void push(struct Node* node) {
    if (top < MAX - 1) {
        stack[++top] = node;
    }
}

struct Node* pop() {
    if (top >= 0) {
        return stack[top--];
    }
    return NULL;
}

int isEmpty() {
    return top == -1;
}
void preorderTraversal(struct Node* root) {
    if (root == NULL) return;

    push(root);

    while (!isEmpty()) {
        struct Node* current = pop();
        printf("%d ", current->data);
        if (current->right) 
          push(current->right);
        if (current->left) 
          push(current->left);
    }
}
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}
int main() {
    struct Node* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    // root->right->left = createNode(6);
    // root->right->right = createNode(7);
    // root->left->left->left = createNode(8);
    // root->left->left->right = createNode(8);
    printf("Preorder Traversal: ");
    preorderTraversal(root);
    printf("\n");
    return 0;
}