#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MIN_DEGREE 3  // t: max keys = 2*t - 1, max children = 2*t

typedef struct BPlusNode {
    int *keys;
    struct BPlusNode **children;
    int num_keys;
    bool leaf;
    struct BPlusNode *next;  // Linked list pointer for leaf-level traversal
} BPlusNode;

typedef struct {
    BPlusNode *root;
    int t;
} BPlusTree;

// ---------- Node Creation ----------

BPlusNode* createNode(int t, bool leaf) {
    BPlusNode* node = malloc(sizeof(BPlusNode));
    node->leaf = leaf;
    node->num_keys = 0;
    node->keys = malloc((2*t - 1) * sizeof(int));
    node->children = malloc((2*t) * sizeof(BPlusNode*));
    node->next = NULL;
    return node;
}

BPlusTree* createTree(int t) {
    BPlusTree* tree = malloc(sizeof(BPlusTree));
    tree->t = t;
    tree->root = createNode(t, true);
    return tree;
}

// ---------- Searching ----------

bool searchItem(BPlusTree* tree, int key) {
    BPlusNode* cur = tree->root;
    while (!cur->leaf) {
        int i = 0;
        while (i < cur->num_keys && key >= cur->keys[i]) i++;
        cur = cur->children[i];
    }
    for (int j = 0; j < cur->num_keys; j++)
        if (cur->keys[j] == key) return true;
    return false;
}

// ---------- Insertion (with splitting) ----------

void splitChild(BPlusNode* parent, int idx, int t) {
    BPlusNode* y = parent->children[idx];
    BPlusNode* z = createNode(t, y->leaf);
    z->num_keys = t - 1;

    for (int j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];
    if (!y->leaf) {
        for (int j = 0; j < t; j++)
            z->children[j] = y->children[j + t];
    }
    y->num_keys = t - 1;

    for (int j = parent->num_keys; j >= idx + 1; j--)
        parent->children[j + 1] = parent->children[j];
    parent->children[idx + 1] = z;
    for (int j = parent->num_keys - 1; j >= idx; j--)
        parent->keys[j + 1] = parent->keys[j];
    parent->keys[idx] = y->keys[t - 1];
    parent->num_keys++;

    if (y->leaf) {
        z->next = y->next;
        y->next = z;
    }
}

void insertNonFull(BPlusNode* node, int key, int t) {
    int i = node->num_keys - 1;
    if (node->leaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        while (i >= 0 && key < node->keys[i]) i--;
        i++;
        if (node->children[i]->num_keys == 2*t - 1) {
            splitChild(node, i, t);
            if (key > node->keys[i]) i++;
        }
        insertNonFull(node->children[i], key, t);
    }
}

void insertItem(BPlusTree* tree, int key) {
    BPlusNode* r = tree->root;
    int t = tree->t;
    if (r->num_keys == 2*t - 1) {
        BPlusNode* s = createNode(t, false);
        tree->root = s;
        s->children[0] = r;
        splitChild(s, 0, t);
        insertNonFull(s, key, t);
    } else {
        insertNonFull(r, key, t);
    }
}

// ---------- Deletion (with merging & borrowing) ----------

int findKeyIndex(BPlusNode* node, int key) {
    int idx = 0;
    while (idx < node->num_keys && node->keys[idx] < key)
        idx++;
    return idx;
}

void borrowFromPrev(BPlusNode* parent, int idx, int t) {
    BPlusNode* child = parent->children[idx];
    BPlusNode* left = parent->children[idx - 1];

    for (int i = child->num_keys - 1; i >= 0; i--)
        child->keys[i + 1] = child->keys[i];
    if (!child->leaf) {
        for (int i = child->num_keys; i >= 0; i--)
            child->children[i + 1] = child->children[i];
    }

    child->keys[0] = parent->keys[idx - 1];
    if (!child->leaf)
        child->children[0] = left->children[left->num_keys];

    parent->keys[idx - 1] = left->keys[left->num_keys - 1];
    child->num_keys++;
    left->num_keys--;
}

void borrowFromNext(BPlusNode* parent, int idx, int t) {
    BPlusNode* child = parent->children[idx];
    BPlusNode* right = parent->children[idx + 1];

    child->keys[child->num_keys] = parent->keys[idx];
    if (!child->leaf)
        child->children[child->num_keys + 1] = right->children[0];

    parent->keys[idx] = right->keys[0];
    for (int i = 1; i < right->num_keys; i++)
        right->keys[i - 1] = right->keys[i];
    if (!right->leaf) {
        for (int i = 1; i <= right->num_keys; i++)
            right->children[i - 1] = right->children[i];
    }

    child->num_keys++;
    right->num_keys--;
}

void mergeNodes(BPlusNode* parent, int idx, int t) {
    BPlusNode* child = parent->children[idx];
    BPlusNode* sibling = parent->children[idx + 1];
    int total = child->num_keys;

    if (!child->leaf) {
        child->keys[total] = parent->keys[idx];
        total++;
    }
    for (int i = 0; i < sibling->num_keys; i++)
        child->keys[total + i] = sibling->keys[i];
    if (!child->leaf) {
        for (int i = 0; i <= sibling->num_keys; i++)
            child->children[total + i] = sibling->children[i];
    }
    if (child->leaf) {
        child->next = sibling->next;
    }
    child->num_keys += sibling->num_keys + (child->leaf ? 0 : 1);

    for (int i = idx + 1; i < parent->num_keys; i++)
        parent->keys[i - 1] = parent->keys[i];
    for (int i = idx + 2; i <= parent->num_keys; i++)
        parent->children[i - 1] = parent->children[i];
    parent->num_keys--;

    free(sibling->keys);
    free(sibling->children);
    free(sibling);
}

void deleteRecursive(BPlusTree* tree, BPlusNode* node, int key) {
    int t = tree->t, idx = findKeyIndex(node, key);

    if (node->leaf) {
        if (idx < node->num_keys && node->keys[idx] == key) {
            for (int i = idx + 1; i < node->num_keys; i++)
                node->keys[i - 1] = node->keys[i];
            node->num_keys--;
        }
        return;
    }

    bool inNode = (idx < node->num_keys && node->keys[idx] == key);
    int childIdx = idx + (inNode ? 1 : 0);
    deleteRecursive(tree, node->children[childIdx], key);

    BPlusNode* child = node->children[childIdx];
    if (child->num_keys < t - 1) {
        bool leftExists = childIdx > 0;
        bool rightExists = childIdx < node->num_keys;
        if (leftExists && node->children[childIdx - 1]->num_keys >= t)
            borrowFromPrev(node, childIdx, t);
        else if (rightExists && node->children[childIdx + 1]->num_keys >= t)
            borrowFromNext(node, childIdx, t);
        else if (leftExists)
            mergeNodes(node, childIdx - 1, t);
        else
            mergeNodes(node, childIdx, t);
    }
}

void deleteItem(BPlusTree* tree, int key) {
    deleteRecursive(tree, tree->root, key);
    if (tree->root->num_keys == 0 && !tree->root->leaf) {
        BPlusNode* old = tree->root;
        tree->root = tree->root->children[0];
        free(old->keys);
        free(old->children);
        free(old);
    }
}

// ---------- Memory Cleanup ----------

void deleteTree(BPlusNode* node) {
    if (!node) return;
    if (!node->leaf) {
        for (int i = 0; i <= node->num_keys; i++)
            deleteTree(node->children[i]);
    }
    free(node->keys);
    free(node->children);
    free(node);
}

// ---------- Leaf Printer (for debugging) ----------

void printLeaves(BPlusTree* tree) {
    BPlusNode* cur = tree->root;
    while (!cur->leaf) cur = cur->children[0];
    printf("Leaf keys: ");
    while (cur) {
        for (int i = 0; i < cur->num_keys; i++)
            printf("%d ", cur->keys[i]);
        cur = cur->next;
    }
    printf("\n");
}

// ---------- Demo in main() ----------

int main() {
    BPlusTree* tree = createTree(MIN_DEGREE);

    int items[] = {10, 20, 5, 6, 12, 30, 7, 17};
    for (int i = 0; i < sizeof(items)/sizeof(items[0]); i++)
        insertItem(tree, items[i]);

    printLeaves(tree);

    int key = 12;
    printf("Searching %d: %s\n", key, searchItem(tree, key) ? "Found" : "Not Found");
    printf("Deleting %d...\n", key);
    deleteItem(tree, key);
    printLeaves(tree);
    printf("Searching %d: %s\n", key, searchItem(tree, key) ? "Still Found" : "Correctly Not Found");

    deleteTree(tree->root);
    free(tree);
    return 0;
}
