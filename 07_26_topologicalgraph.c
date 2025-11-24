#include <stdio.h>
#include <stdlib.h>

#define MAX 10000

typedef struct Node {
    int v;
    struct Node* next;
} Node;

Node* adj[MAX]; 
int V;          

void addEdge(int u, int v) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->v = v;
    n->next = adj[u];
    adj[u] = n;
}

int visited[MAX];
int stack[MAX];
int top = -1;

void dfsTopo(int v) {
    visited[v] = 1;
    for (Node* n = adj[v]; n; n = n->next) {
        if (!visited[n->v])
            dfsTopo(n->v);
    }
    stack[++top] = v; 
}

void topologicalSort() {
    for (int i = 0; i < V; i++)
        visited[i] = 0;

    for (int i = 0; i < V; i++) {
        if (!visited[i])
            dfsTopo(i);
    }

    printf("Topological Order:\n");
    while (top >= 0)
        printf("%d ", stack[top--]);
    printf("\n");
}

int main() {
    V = 6; 
    addEdge(5, 2);
    addEdge(5, 0);
    addEdge(4, 0);
    addEdge(4, 1);
    addEdge(2, 3);
    addEdge(3, 1);

    topologicalSort();

    return 0;
}