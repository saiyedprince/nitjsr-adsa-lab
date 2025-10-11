#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N 4
#define SIZE (N * N)

typedef struct Node {
    int mat[N][N];
    int x, y; // blank tile position
    int cost;
    int level;
    struct Node* parent;
} Node;

// Goal state
int goal[N][N] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 0}
};

// Directions: up, down, left, right
int row[] = { -1, 1, 0, 0 };
int col[] = { 0, 0, -1, 1 };

// Function to calculate Manhattan distance
int calculateCost(int mat[N][N]) {
    int cost = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (mat[i][j] != 0) {
                int targetX = (mat[i][j] - 1) / N;
                int targetY = (mat[i][j] - 1) % N;
                cost += abs(i - targetX) + abs(j - targetY);
            }
    return cost;
}

// Create a new node
Node* newNode(int mat[N][N], int x, int y, int newX, int newY, int level, Node* parent) {
    Node* node = (Node*)malloc(sizeof(Node));
    memcpy(node->mat, mat, sizeof(node->mat));
    int temp = node->mat[x][y];
    node->mat[x][y] = node->mat[newX][newY];
    node->mat[newX][newY] = temp;
    node->cost = calculateCost(node->mat);
    node->level = level;
    node->x = newX;
    node->y = newY;
    node->parent = parent;
    return node;
}

// Print the board
void printMatrix(int mat[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%2d ", mat[i][j]);
        printf("\n");
    }
    printf("\n");
}

// Priority queue node comparison
int compare(const void* a, const void* b) {
    Node* n1 = *(Node**)a;
    Node* n2 = *(Node**)b;
    return (n1->cost + n1->level) - (n2->cost + n2->level);
}

// Solve the puzzle
void solve(int initial[N][N], int x, int y) {
    Node* root = newNode(initial, x, y, x, y, 0, NULL);
    Node* heap[10000];
    int heapSize = 0;

    heap[heapSize++] = root;

    while (heapSize > 0) {
        qsort(heap, heapSize, sizeof(Node*), compare);
        Node* min = heap[0];

        if (min->cost == 0) {
            printf("Solution found in %d moves:\n\n", min->level);
            Node* temp = min;
            while (temp) {
                printMatrix(temp->mat);
                temp = temp->parent;
            }
            return;
        }

        // Remove min from heap
        for (int i = 1; i < heapSize; i++)
            heap[i - 1] = heap[i];
        heapSize--;

        for (int i = 0; i < 4; i++) {
            int newX = min->x + row[i];
            int newY = min->y + col[i];

            if (newX >= 0 && newX < N && newY >= 0 && newY < N) {
                Node* child = newNode(min->mat, min->x, min->y, newX, newY, min->level + 1, min);
                heap[heapSize++] = child;
            }
        }
    }

    printf("No solution found.\n");
}

int main() {
    int initial[N][N] = {
        {1, 2, 3, 4},
        {5, 6, 0, 8},
        {9, 10, 7, 12},
        {13, 14, 11, 15}
    };

    int x = 1, y = 2; // blank tile position

    solve(initial, x, y);
    return 0;
}