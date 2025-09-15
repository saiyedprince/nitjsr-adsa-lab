#include <stdio.h>
#include <stdlib.h>

typedef struct AVLNode {
    int key;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(AVLNode *node) {
    return (node == NULL) ? 0 : node->height;
}

AVLNode* createNode(int key) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1; 
    return node;
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(AVLNode* node) {
    return (node == NULL) ? 0 : height(node->left) - height(node->right);
}


AVLNode* createTree() {
    return NULL;
}

AVLNode* insertItem(AVLNode* node, int key) {
    if (node == NULL)
        return createNode(key);

    if (key < node->key)
        node->left = insertItem(node->left, key);
    else if (key > node->key)
        node->right = insertItem(node->right, key);
    else
        return node; 

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

AVLNode* minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

AVLNode* deleteItem(AVLNode* root, int key) {
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = deleteItem(root->left, key);
    else if (key > root->key)
        root->right = deleteItem(root->right, key);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;

            free(temp);
        } else {
            AVLNode* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteItem(root->right, temp->key);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

AVLNode* searchItem(AVLNode* root, int key) {
    if (root == NULL || root->key == key)
        return root;
    if (key < root->key)
        return searchItem(root->left, key);
    else
        return searchItem(root->right, key);
}

void deleteTree(AVLNode* root) {
    if (root == NULL)
        return;
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}

void inOrder(AVLNode* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%d ", root->key);
        inOrder(root->right);
    }
}

int main() {
    AVLNode* root = createTree();

    root = insertItem(root, 20);
    root = insertItem(root, 10);
    root = insertItem(root, 30);
    root = insertItem(root, 5);
    root = insertItem(root, 15);
    root = insertItem(root, 25);
    root = insertItem(root, 35);

    printf("In-order traversal: ");
    inOrder(root);
    printf("\n");

    AVLNode* found = searchItem(root, 25);
    if (found)
        printf("Item 25 found in tree.\n");
    else
        printf("Item 25 not found.\n");

    root = deleteItem(root, 10);
    printf("After deleting 10: ");
    inOrder(root);
    printf("\n");

    deleteTree(root);
    return 0;
}
