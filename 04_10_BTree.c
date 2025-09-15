#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define T 3  // Minimum degree

typedef struct BTreeNode {
    int keys[2 * T - 1];              // Array of keys
    struct BTreeNode* children[2 * T]; // Array of child pointers
    int numKeys;                      // Current number of keys
    bool isLeaf;                      // Is true when node is leaf
} BTreeNode;

// ====== Function Prototypes ======
BTreeNode* createTree();
void deleteTree(BTreeNode* node);
BTreeNode* insertItem(BTreeNode* root, int key);
BTreeNode* deleteItem(BTreeNode* root, int key);
BTreeNode* searchItem(BTreeNode* root, int key);
void printTree(BTreeNode* node, int indent);

// ====== Utility Functions ======
BTreeNode* createNode(bool isLeaf) {
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->isLeaf = isLeaf;
    node->numKeys = 0;
    for (int i = 0; i < 2 * T; i++)
        node->children[i] = NULL;
    return node;
}

BTreeNode* createTree() {
    return createNode(true);
}

void deleteTree(BTreeNode* node) {
    if (node == NULL) return;
    if (!node->isLeaf) {
        for (int i = 0; i <= node->numKeys; i++)
            deleteTree(node->children[i]);
    }
    free(node);
}

// ====== Search ======
BTreeNode* searchItem(BTreeNode* root, int key) {
    int i = 0;
    while (i < root->numKeys && key > root->keys[i])
        i++;

    if (i < root->numKeys && key == root->keys[i])
        return root;

    if (root->isLeaf)
        return NULL;

    return searchItem(root->children[i], key);
}

// ====== Insertion ======

// Splits the child of a node
void splitChild(BTreeNode* parent, int i) {
    BTreeNode* fullChild = parent->children[i];
    BTreeNode* newChild = createNode(fullChild->isLeaf);
    newChild->numKeys = T - 1;

    for (int j = 0; j < T - 1; j++)
        newChild->keys[j] = fullChild->keys[j + T];

    if (!fullChild->isLeaf) {
        for (int j = 0; j < T; j++)
            newChild->children[j] = fullChild->children[j + T];
    }

    fullChild->numKeys = T - 1;

    for (int j = parent->numKeys; j >= i + 1; j--)
        parent->children[j + 1] = parent->children[j];
    parent->children[i + 1] = newChild;

    for (int j = parent->numKeys - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];
    parent->keys[i] = fullChild->keys[T - 1];

    parent->numKeys++;
}

void insertNonFull(BTreeNode* node, int key) {
    int i = node->numKeys - 1;

    if (node->isLeaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->numKeys++;
    } else {
        while (i >= 0 && key < node->keys[i])
            i--;

        if (node->children[i + 1]->numKeys == 2 * T - 1) {
            splitChild(node, i + 1);
            if (key > node->keys[i + 1])
                i++;
        }
        insertNonFull(node->children[i + 1], key);
    }
}

BTreeNode* insertItem(BTreeNode* root, int key) {
    if (root->numKeys == 2 * T - 1) {
        BTreeNode* newRoot = createNode(false);
        newRoot->children[0] = root;
        splitChild(newRoot, 0);
        insertNonFull(newRoot, key);
        return newRoot;
    } else {
        insertNonFull(root, key);
        return root;
    }
}

// ====== Deletion ======
// Note: B-tree deletion is complex. This implementation covers all cases.

int getPred(BTreeNode* node) {
    while (!node->isLeaf)
        node = node->children[node->numKeys];
    return node->keys[node->numKeys - 1];
}

int getSucc(BTreeNode* node) {
    while (!node->isLeaf)
        node = node->children[0];
    return node->keys[0];
}

void merge(BTreeNode* node, int idx) {
    BTreeNode* child = node->children[idx];
    BTreeNode* sibling = node->children[idx + 1];

    child->keys[T - 1] = node->keys[idx];

    for (int i = 0; i < sibling->numKeys; ++i)
        child->keys[i + T] = sibling->keys[i];

    if (!child->isLeaf) {
        for (int i = 0; i <= sibling->numKeys; ++i)
            child->children[i + T] = sibling->children[i];
    }

    for (int i = idx + 1; i < node->numKeys; ++i)
        node->keys[i - 1] = node->keys[i];
    for (int i = idx + 2; i <= node->numKeys; ++i)
        node->children[i - 1] = node->children[i];

    child->numKeys += sibling->numKeys + 1;
    node->numKeys--;

    free(sibling);
}

void fill(BTreeNode* node, int idx);

void removeFromLeaf(BTreeNode* node, int idx) {
    for (int i = idx + 1; i < node->numKeys; ++i)
        node->keys[i - 1] = node->keys[i];
    node->numKeys--;
}

void removeFromNonLeaf(BTreeNode* node, int idx) {
    int key = node->keys[idx];

    if (node->children[idx]->numKeys >= T) {
        int pred = getPred(node->children[idx]);
        node->keys[idx] = pred;
        node->children[idx] = deleteItem(node->children[idx], pred);
    } else if (node->children[idx + 1]->numKeys >= T) {
        int succ = getSucc(node->children[idx + 1]);
        node->keys[idx] = succ;
        node->children[idx + 1] = deleteItem(node->children[idx + 1], succ);
    } else {
        merge(node, idx);
        node->children[idx] = deleteItem(node->children[idx], key);
    }
}

BTreeNode* deleteItem(BTreeNode* root, int key) {
    int idx = 0;
    while (idx < root->numKeys && key > root->keys[idx])
        idx++;

    if (idx < root->numKeys && root->keys[idx] == key) {
        if (root->isLeaf)
            removeFromLeaf(root, idx);
        else
            removeFromNonLeaf(root, idx);
    } else {
        if (root->isLeaf)
            return root;

        bool flag = (idx == root->numKeys);

        if (root->children[idx]->numKeys < T)
            fill(root, idx);

        if (flag && idx > root->numKeys)
            root->children[idx - 1] = deleteItem(root->children[idx - 1], key);
        else
            root->children[idx] = deleteItem(root->children[idx], key);
    }

    if (root->numKeys == 0) {
        BTreeNode* tmp = root;
        if (root->isLeaf) {
            free(root);
            return NULL;
        } else {
            root = root->children[0];
            free(tmp);
        }
    }

    return root;
}

void borrowFromPrev(BTreeNode* node, int idx) {
    BTreeNode* child = node->children[idx];
    BTreeNode* sibling = node->children[idx - 1];

    for (int i = child->numKeys - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];

    if (!child->isLeaf) {
        for (int i = child->numKeys; i >= 0; --i)
            child->children[i + 1] = child->children[i];
    }

    child->keys[0] = node->keys[idx - 1];

    if (!child->isLeaf)
        child->children[0] = sibling->children[sibling->numKeys];

    node->keys[idx - 1] = sibling->keys[sibling->numKeys - 1];

    child->numKeys += 1;
    sibling->numKeys -= 1;
}

void borrowFromNext(BTreeNode* node, int idx) {
    BTreeNode* child = node->children[idx];
    BTreeNode* sibling = node->children[idx + 1];

    child->keys[child->numKeys] = node->keys[idx];

    if (!child->isLeaf)
        child->children[child->numKeys + 1] = sibling->children[0];

    node->keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->numKeys; ++i)
        sibling->keys[i - 1] = sibling->keys[i];
    if (!sibling->isLeaf) {
        for (int i = 1; i <= sibling->numKeys; ++i)
            sibling->children[i - 1] = sibling->children[i];
    }

    child->numKeys += 1;
    sibling->numKeys -= 1;
}

void fill(BTreeNode* node, int idx) {
    if (idx != 0 && node->children[idx - 1]->numKeys >= T)
        borrowFromPrev(node, idx);
    else if (idx != node->numKeys && node->children[idx + 1]->numKeys >= T)
        borrowFromNext(node, idx);
    else {
        if (idx != node->numKeys)
            merge(node, idx);
        else
            merge(node, idx - 1);
    }
}

// ====== Tree Printing (for Testing) ======
void printTree(BTreeNode* node, int indent) {
    if (node == NULL) return;

    for (int i = 0; i < node->numKeys; i++) {
        if (!node->isLeaf)
            printTree(node->children[i], indent + 4);
        for (int j = 0; j < indent; j++) printf(" ");
        printf("%d ", node->keys[i]);
    }
    printf("\n");

    if (!node->isLeaf)
        printTree(node->children[node->numKeys], indent + 4);
}

// ====== Main for Testing ======
int main() {
    BTreeNode* root = createTree();

    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int n = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < n; i++)
        root = insertItem(root, keys[i]);

    printf("B-tree structure:\n");
    printTree(root, 0);

    int searchKey = 6;
    if (searchItem(root, searchKey))
        printf("Key %d found in B-tree.\n", searchKey);
    else
        printf("Key %d not found.\n", searchKey);

    root = deleteItem(root, 6);
    printf("After deleting 6:\n");
    printTree(root, 0);

    deleteTree(root);
    return 0;
}
