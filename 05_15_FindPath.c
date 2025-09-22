#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 100

int V;  // number of vertices
int isDirected = 0; // 0 = undirected, 1 = directed

// Adjacency list
struct Node {
    int dest;
    struct Node* next;
};

struct Node* adj[MAX];
int visited[MAX];
int path[MAX];       // current path
int pathLength = 0;
int minCycle = INT_MAX;
int maxCycle = 0;

// Add edge to graph
void addEdge(int u, int v) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->dest = v;
    node->next = adj[u];
    adj[u] = node;

    if (!isDirected) {
        struct Node* node2 = (struct Node*)malloc(sizeof(struct Node));
        node2->dest = u;
        node2->next = adj[v];
        adj[v] = node2;
    }
}

// DFS backtracking to detect cycles
void DFS(int start, int current) {
    visited[current] = 1;
    path[pathLength++] = current;

    struct Node* temp = adj[current];
    while (temp) {
        int neighbor = temp->dest;

        // Cycle detected
        if (neighbor == start && pathLength >= 3) {
            if (pathLength < minCycle) minCycle = pathLength;
            if (pathLength > maxCycle) maxCycle = pathLength;
        }
        // Not visited and not revisiting parent in undirected graph
        else if (!visited[neighbor]) {
            DFS(start, neighbor);
        }

        temp = temp->next;
    }

    visited[current] = 0;
    pathLength--;
}

// Wrapper function
void findCycles() {
    minCycle = INT_MAX;
    maxCycle = 0;

    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) visited[j] = 0;
        pathLength = 0;
        DFS(i, i);
    }

    if (minCycle == INT_MAX)
        printf("No cycles found.\n");
    else {
        printf("Smallest cycle length: %d\n", minCycle);
        printf("Largest cycle length: %d\n", maxCycle);
    }
}

// Free memory
void resetGraph() {
    for (int i = 0; i < V; i++) {
        struct Node* current = adj[i];
        while (current) {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
        adj[i] = NULL;
    }
}
int main() {
    V = 6;
    isDirected = 1;  // Change to 0 for undirected graph

    // Directed Graph Example
    addEdge(0, 1);
    addEdge(1, 2);
    addEdge(2, 0);  // cycle of length 3

    addEdge(2, 3);
    addEdge(3, 4);
    addEdge(4, 2);  // cycle of length 4

    addEdge(4, 5);  // no cycle here

    findCycles();

    resetGraph();
    return 0;
}
