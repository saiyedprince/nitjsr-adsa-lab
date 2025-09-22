#include <stdio.h>
#include <stdlib.h>

#define MAX 100

enum { WHITE, GRAY, BLACK }; // Node states

// Adjacency list node
struct Node {
    int dest;
    struct Node* next;
};

// Adjacency list
struct Node* adj[MAX];
int V;  // number of vertices

// DFS tracking arrays
int color[MAX], d[MAX], f[MAX], time = 0;

// Add edge to graph
void addEdge(int u, int v) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->dest = v;
    node->next = adj[u];
    adj[u] = node;
}

// DFS visit function
void DFSVisit(int u) {
    color[u] = GRAY;
    d[u] = ++time;

    struct Node* temp = adj[u];
    while (temp != NULL) {
        int v = temp->dest;

        if (color[v] == WHITE) {
            printf("Tree Edge: %d -> %d\n", u, v);
            DFSVisit(v);
        }
        else if (color[v] == GRAY) {
            printf("Back Edge: %d -> %d\n", u, v);
        }
        else if (color[v] == BLACK) {
            if (d[u] < d[v])
                printf("Forward Edge: %d -> %d\n", u, v);
            else
                printf("Cross Edge: %d -> %d\n", u, v);
        }

        temp = temp->next;
    }

    color[u] = BLACK;
    f[u] = ++time;
}

// Main DFS function
void DFS() {
    for (int i = 0; i < V; i++) {
        color[i] = WHITE;
        d[i] = f[i] = 0;
    }

    time = 0;

    for (int i = 0; i < V; i++) {
        if (color[i] == WHITE)
            DFSVisit(i);
    }
}

// Helper function to reset graph
void resetGraph() {
    for (int i = 0; i < V; i++) {
        struct Node* current = adj[i];
        while (current != NULL) {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
        adj[i] = NULL;
    }
}
int main() {
    V = 6; // number of vertices

    // Add edges (directed graph)
    addEdge(0, 1);
    addEdge(0, 2);
    addEdge(1, 3);
    addEdge(1, 4);
    addEdge(2, 4);
    addEdge(3, 5);
    addEdge(4, 3);
    addEdge(5, 1);  // Back edge

    DFS();

    resetGraph();  // free memory
    return 0;
}
