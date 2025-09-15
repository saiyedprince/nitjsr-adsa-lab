#include <stdio.h>
#include <stdlib.h>

// Enum for colors
typedef enum { RED, BLACK } Color;

// Node structure
typedef struct Node {
    int key;
    Color color;
    struct Node *parent, *left, *right;
} Node;

// Tree structure
typedef struct {
    Node *root;
    Node *nil;  // Sentinel nil node
} RBTree;

// Create a new node
Node* createNode(RBTree *T, int key, Color color) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->color = color;
    node->left = T->nil;
    node->right = T->nil;
    node->parent = T->nil;
    return node;
}

// Create a red-black tree
RBTree* createTree() {
    RBTree *T = (RBTree*)malloc(sizeof(RBTree));
    T->nil = (Node*)malloc(sizeof(Node));
    T->nil->color = BLACK;
    T->nil->left = T->nil->right = T->nil->parent = NULL;
    T->root = T->nil;
    return T;
}

// Left rotate
void leftRotate(RBTree *T, Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != T->nil) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == T->nil) T->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Right rotate
void rightRotate(RBTree *T, Node *y) {
    Node *x = y->left;
    y->left = x->right;
    if (x->right != T->nil) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == T->nil) T->root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}

// Insert fixup
void insertFixup(RBTree *T, Node *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node *y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(T, z->parent->parent);
            }
        } else {
            Node *y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(T, z->parent->parent);
            }
        }
    }
    T->root->color = BLACK;
}

// Insert key
void insertItem(RBTree *T, int key) {
    Node *z = createNode(T, key, RED);
    Node *y = T->nil;
    Node *x = T->root;
    while (x != T->nil) {
        y = x;
        if (z->key < x->key) x = x->left;
        else x = x->right;
    }
    z->parent = y;
    if (y == T->nil) T->root = z;
    else if (z->key < y->key) y->left = z;
    else y->right = z;
    insertFixup(T, z);
}

// Search key
Node* searchItem(RBTree *T, Node *x, int key) {
    while (x != T->nil && key != x->key) {
        if (key < x->key) x = x->left;
        else x = x->right;
    }
    return x;
}

// Minimum node
Node* treeMinimum(RBTree *T, Node *x) {
    while (x->left != T->nil)
        x = x->left;
    return x;
}

// Transplant
void rbTransplant(RBTree *T, Node *u, Node *v) {
    if (u->parent == T->nil)
        T->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

// Delete fixup
void deleteFixup(RBTree *T, Node *x) {
    while (x != T->root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(T, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(T, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(T, x->parent);
                x = T->root;
            }
        } else {
            Node *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(T, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(T, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(T, x->parent);
                x = T->root;
            }
        }
    }
    x->color = BLACK;
}

// Delete key
void deleteItem(RBTree *T, int key) {
    Node *z = searchItem(T, T->root, key);
    if (z == T->nil) return;

    Node *y = z;
    Color yOriginalColor = y->color;
    Node *x;

    if (z->left == T->nil) {
        x = z->right;
        rbTransplant(T, z, z->right);
    } else if (z->right == T->nil) {
        x = z->left;
        rbTransplant(T, z, z->left);
    } else {
        y = treeMinimum(T, z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z)
            x->parent = y;
        else {
            rbTransplant(T, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        rbTransplant(T, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    free(z);
    if (yOriginalColor == BLACK)
        deleteFixup(T, x);
}

// Inorder traversal
void inorder(RBTree *T, Node *x) {
    if (x == T->nil) return;
    inorder(T, x->left);
    printf("%d(%c) ", x->key, x->color == RED ? 'R' : 'B');
    inorder(T, x->right);
}

// Free tree nodes
void freeTreeNodes(RBTree *T, Node *x) {
    if (x == T->nil) return;
    freeTreeNodes(T, x->left);
    freeTreeNodes(T, x->right);
    free(x);
}

// Delete the tree
void deleteTree(RBTree *T) {
    freeTreeNodes(T, T->root);
    free(T->nil);
    free(T);
}

// Main function
int main() {
    RBTree *T = createTree();

    int keys[] = {20, 15, 25, 10, 5, 30, 28};
    int n = sizeof(keys) / sizeof(keys[0]);
    for (int i = 0; i < n; i++) {
        insertItem(T, keys[i]);
    }

    printf("Tree after inserts (in-order traversal):\n");
    inorder(T, T->root);
    printf("\n");

    printf("Searching for 15: %s\n", searchItem(T, T->root, 15) != T->nil ? "Found" : "Not Found");

    deleteItem(T, 15);
    printf("Tree after deleting 15:\n");
    inorder(T, T->root);
    printf("\n");

    deleteTree(T);
    return 0;
}
